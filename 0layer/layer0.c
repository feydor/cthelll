#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCODESIZE 3333
#define NON_NUMERIC INT_MAX
#define NIL -1
FILE *fp;
char code[MAXCODESIZE];
char *tokens[MAXCODESIZE/3];
int jmptable[MAXCODESIZE];
int stack[333] = {NIL};
int sp = 0;
char *unaryops[] = {"jmp", "ret", "call", "#", "push", "pop"};
char *binaryops[] = {"add", "sub", "mov", "xor", "mul", "mod"};
char *ternaryops[] = {"beq", "bne", "bgt", "blt"};
char regs[] = "abcdefghijklmnopqrstuvwxyz";
int64_t mem[26] = {0};
int codelen, lines;
int hash(char *s) {
    int n = 0;
    while (*s) {
        n += *s;
        s++;
    }
    return n;
}

bool strfind(char *s, char c) {
    while (*s) {
        if (*s == c) return true;
        s++;
    }
    return false; 
}

int strnum(char *s) {
    int n = 0;
    int places = strlen(s);
    while (*s) {
        if (!isdigit(*s)) return NON_NUMERIC;
        n += ((*s)-48) * pow(10, places-1);
        s++;
        places--;
    }
    return n;
}

bool isregister(char *operand) {
    return strlen(operand) == 1 && strfind(regs, operand[0]);
}

bool isnumeric(char *operand) {
    while (*operand) {
        if (!isdigit(*operand)) return false;
        operand++;
    }
    return true;
}

bool islabel(char *operand) {
    return jmptable[hash(operand)+':'] != NIL;
}

bool isstackptr(char *operand) {
    return operand[0] == 's' && operand[1] == 'p';
}

int jmptable_lookup(char *label) {
    return jmptable[hash(label)+':'];
}

// " tail"
char* trim_ws(char *s) {
    int start = 0;
    while(isspace(*s)) {
        start++;
        s++;
    }
    return s;
}

void concat(char *x, char *y) {
    size_t xlen = strlen(x);
    char new[xlen + strlen(y) + 1];
    memcpy(new, x, xlen);
    memcpy(new+xlen, y, strlen(y));
    new[sizeof(new)-1] = '\0';
    strcpy(x, new);
}

// returns the index, if not found returns -1
size_t findstr(size_t n, char *arr[n], char *str) {
    size_t pos = -1;
    for (int i = 0; i < n; ++i)
        if (!strcmp(str, arr[i])) pos = i;
    return pos;
}

size_t unaryop(char *op) {
    return findstr(sizeof(unaryops)/sizeof(unaryops[0]), unaryops, op);
}
size_t binaryop(char *op) {
    return findstr(sizeof(binaryops)/sizeof(binaryops[0]), binaryops, op);
}
size_t ternaryop(char *op) {
    return findstr(sizeof(ternaryops)/sizeof(ternaryops[0]), ternaryops, op);
}

// NOTE: only single digit offsets
int stackptr_offset(char *param) {
    return isstackptr(param) && isdigit(param[strlen(param)-1]) ? param[strlen(param)-1]-'0' : 0;
}
            
void eval_unary(size_t op_idx, char *param, size_t *curr_tok) {
    // error when encountering comma
    char *op = unaryops[op_idx];
    if (!strcmp("jmp", op)) {
        *curr_tok = jmptable_lookup(param);
    } else if (!strcmp("#", op)) {
        if (isnumeric(param)) {
            printf("%s\n", param);
        } else if (isregister(param)){
            printf("%s: %ld\n", param, mem[param[0]-'a']);
        } else if (isstackptr(param)) {
            int offset = stackptr_offset(param);
            printf("%s: %d\n", param, stack[sp - offset]);
        }
    } else if (!strcmp("call", op)) {
        stack[++sp] = (int)*curr_tok;
        if (!islabel(param)) fprintf(stderr, "call procedure must have a valid label: '%s'\n", param), exit(-1);
        *curr_tok = jmptable_lookup(param);
    } else if (!strcmp("ret", op)) {
        *curr_tok = (size_t)stack[sp];
        sp--;
        if (*curr_tok == NIL) fprintf(stderr, "Ret statement without corresponding call\n"), exit(-1);
    } else if (!strcmp("push", op)) {
        // param can be reg or number
        if (isregister(param)) {
            stack[++sp] = mem[param[0]-'a'];
        } else {
            stack[++sp] = strnum(param);
        }
    } else if (!strcmp("pop", op)) {
        // param must be a number
        if (!isnumeric(param)) fprintf(stderr, "pop's param must be numeric: '%s'\n", param), exit(-1);
        int to_pop = strnum(param);    
        if (sp-to_pop < 0) fprintf(stderr, "Stack underflow: %d\n", sp-to_pop), exit(-1);
        sp -= to_pop;
    } else {
        printf("unary operator, %s, not yet implemented\n", op);
    }
}

void eval_binary(size_t op_idx, char *params) {
    char *op = binaryops[op_idx];
    char *params_cpy = strdup(params);
    char *lparam = strtok(params_cpy, ",");
    char *rparam = strtok(NULL, ",");
    if (!lparam || !rparam) {
        fprintf(stderr, "Parse error in binary op: '%s'\n", op), exit(-1);
    }

    size_t lreg = lparam[0]-'a';
    size_t rreg = rparam[0]-'a';
    int loffset = stackptr_offset(lparam);
    int roffset = stackptr_offset(rparam);
    if (sp - loffset < 0) fprintf(stderr, "Stack access underflow: sp: %d, loffset: %d\n", sp, loffset), exit(-1);
    if (sp - roffset < 0) fprintf(stderr, "Stack access underflow: sp: %d, roffset: %d\n", sp, roffset), exit(-1);
    if (!strcmp("mov", op)) {
        if (isregister(lparam)) {
            if (isnumeric(rparam)) {
                mem[lreg] = strnum(rparam);
            } else if (isregister(rparam)) {
                mem[lreg] = mem[rreg];
            } else if (isstackptr(rparam)){
                mem[lreg] = stack[sp - roffset];
            }
        } else if (isstackptr(lparam)) {
            if (isnumeric(rparam)) {
                stack[sp - loffset] = strnum(rparam);
            } else if (isregister(rparam)) {
                stack[sp - loffset] = mem[rreg];
            } else if (isstackptr(rparam)){
                stack[sp - loffset] = stack[sp - roffset];
            }
        }
    } else if (!strcmp("add", op)) {
        if (isregister(lparam)) {
            if (isnumeric(rparam)) {
                mem[lreg] += strnum(rparam);
            } else if (isregister(rparam)) {
                mem[lreg] += mem[rreg];
            } else if (isstackptr(rparam)){
                mem[lreg] += stack[sp - roffset];
            }
        } else if (isstackptr(lparam)) {
            if (isnumeric(rparam)) {
                stack[sp - loffset] += strnum(rparam);
            } else if (isregister(rparam)) {
                stack[sp - loffset] += mem[rreg];
            } else if (isstackptr(rparam)){
                stack[sp - loffset] += stack[sp - roffset];
            }
        }
    } else if (!strcmp("sub", op)) {
        if (isregister(lparam)) {
            if (isnumeric(rparam)) {
                mem[lreg] -= strnum(rparam);
            } else if (isregister(rparam)) {
                mem[lreg] -= mem[rreg];
            } else if (isstackptr(rparam)){
                mem[lreg] -= stack[sp - roffset];
            }
        } else if (isstackptr(lparam)) {
            if (isnumeric(rparam)) {
                stack[sp - loffset] -= strnum(rparam);
            } else if (isregister(rparam)) {
                stack[sp - loffset] -= mem[rreg];
            } else if (isstackptr(rparam)){
                stack[sp - loffset] -= stack[sp - roffset];
            }
        }
    } else if (!strcmp("xor", op)) {
        if (isregister(lparam)) {
            if (isnumeric(rparam)) {
                mem[lreg] ^= strnum(rparam);
            } else if (isregister(rparam)) {
                mem[lreg] ^= mem[rreg];
            } else if (isstackptr(rparam)){
                mem[lreg] ^= stack[sp - roffset];
            }
        } else if (isstackptr(lparam)) {
            if (isnumeric(rparam)) {
                stack[sp - loffset] ^= strnum(rparam);
            } else if (isregister(rparam)) {
                stack[sp - loffset] ^= mem[rreg];
            } else if (isstackptr(rparam)){
                stack[sp - loffset] ^= stack[sp - roffset];
            }
        }
    } else if (!strcmp("mul", op)) {
        if (isregister(lparam)) {
            if (isnumeric(rparam)) {
                mem[lreg] *= strnum(rparam);
            } else if (isregister(rparam)) {
                mem[lreg] *= mem[rreg];
            } else if (isstackptr(rparam)){
                mem[lreg] *= stack[sp - roffset];
            }
        } else if (isstackptr(lparam)) {
            if (isnumeric(rparam)) {
                stack[sp - loffset] *= strnum(rparam);
            } else if (isregister(rparam)) {
                stack[sp - loffset] *= mem[rreg];
            } else if (isstackptr(rparam)){
                stack[sp - loffset] *= stack[sp - roffset];
            }
        }
    } else if (!strcmp("mod", op)) {
        if (isregister(lparam)) {
            if (isnumeric(rparam)) {
                mem[lreg] %= strnum(rparam);
            } else if (isregister(rparam)) {
                mem[lreg] %= mem[rreg];
            } else if (isstackptr(rparam)){
                mem[lreg] %= stack[sp - roffset];
            }
        } else if (isstackptr(lparam)) {
            if (isnumeric(rparam)) {
                stack[sp - loffset] %= strnum(rparam);
            } else if (isregister(rparam)) {
                stack[sp - loffset] %= mem[rreg];
            } else if (isstackptr(rparam)){
                stack[sp - loffset] %= stack[sp - roffset];
            }
        }
    }
}

void eval_ternary(size_t op_idx, char *params, size_t *curr_tok) {
    // params a,b,c can be: reg,reg,label or reg,num,label, or num,num,label
    char *params_cpy = strdup(params);
    char *a = strtok(params_cpy, ",");
    char *b = strtok(NULL, ",");
    char *c = strtok(NULL, ",");
    if (!a || !b || !c) {
        fprintf(stderr, "Parse error in ternary op\n"), exit(-1);
    } else if (!islabel(c)) {
        fprintf(stderr, "Rightmost param in a ternary op must be a label: '%s'\n", c), exit(-1);
    }

    char *op = ternaryops[op_idx];
    int jmptarget = jmptable_lookup(c);
    size_t lreg = a[0]-'a';
    size_t rreg = b[0]-'a';
    if (!strcmp("beq", op)) {
        if (isnumeric(b)) {
            if (mem[lreg] == strnum(b)) *curr_tok = jmptarget;
        } else {
            if (mem[lreg] == mem[rreg]) *curr_tok = jmptarget;
        }
    } else if (!strcmp("bne", op)) {
        if (isnumeric(b)) {
            if (mem[lreg] != strnum(b)) *curr_tok = jmptarget;
        } else {
            if (mem[lreg] != mem[rreg]) *curr_tok = jmptarget;
        }
    } else if (!strcmp("bgt", op)) {
        if (isnumeric(b)) {
            if (mem[lreg] > strnum(b)) *curr_tok = jmptarget;
        } else {
            if (mem[lreg] > mem[rreg]) *curr_tok = jmptarget;
        }
    } else if (!strcmp("blt", op)) {
        if (isnumeric(b)) {
            if (mem[lreg] < strnum(b)) *curr_tok = jmptarget;
        } else {
            if (mem[lreg] < mem[rreg]) *curr_tok = jmptarget;
        }
    } else {
        printf("Ternary operator not implemented: '%s'\n", op);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) printf("program: layer0 [file]\n"), exit(1);
    if (!(fp = fopen(argv[1], "r"))) fprintf(stderr, "error opening the file.\n"), exit(1);
    codelen = fread(code, 1, MAXCODESIZE, fp);
    fclose(fp);
    for (int i = 0; i < sizeof(jmptable)/sizeof(jmptable[0]); ++i)
        jmptable[i] = NIL;

    // first pass
    int tcc = 0;
    int ntokens = 0;
    while (tcc != codelen) {
        int cc = 0;
        while (code[tcc+cc] != '\n') {
            cc++;
        }

        char *line = calloc(cc, sizeof(*line));
        if (!line) {
            fprintf(stderr, "calloc failed\n");
            exit(1);
        }

        memcpy(line, code+tcc, cc);
        line[cc] = '\0';
        line = trim_ws(line);
        char *tok = strtok(line, " ");
        if (!strcmp(tok, "%") || !strcmp(tok, " ")) {
            tcc += cc+1;
            continue;
        } else if (tok[strlen(tok)-1] == ':') {
            if (jmptable[hash(tok)] != NIL) fprintf(stderr, "duplicate label: %d: '%s'\n", lines, tok), exit(1);
            jmptable[hash(tok)] = ntokens;
        }
        
        tokens[ntokens++] = strdup(tok);
        while ((tok = strtok(NULL, " "))) {
            if (!strcmp(tok, "%") || !strcmp(tok, " ")) break;
            tokens[ntokens++] = strdup(tok);
        }
        tcc += cc+1;
    }

    for (size_t i = 0; i < ntokens; ++i) {
        size_t op = -1;
        if ((op = unaryop(tokens[i])) != -1) {
            eval_unary(op, tokens[i+1], &i);
        } else if ((op = binaryop(tokens[i])) != -1) {
            eval_binary(op, tokens[i+1]);
            i++;
        } else if ((op = ternaryop(tokens[i])) != -1) {
            eval_ternary(op, tokens[i+1], &i);
        }
    }
    
    return 0;
}
