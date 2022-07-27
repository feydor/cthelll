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
int jmptable[MAXCODESIZE];
int stack[MAXCODESIZE];
char *tokens[MAXCODESIZE];
int sp = 0;
char *unaryops[] = {"jmp", "ret", "call", "#"};
char *binaryops[] = {"add", "sub", "mov", "xor", "mul", "mod", "and", "or"};
char *ternaryops[] = {"beq", "bne", "bgt", "blt"};
char regs[] = "abcdefghijklmnopqrstuvwxyz";
int mem[26] = {0};
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

void num2reg_op(char *instr, int reg, int n, int line) {
    if (!strcmp("mov", instr)) {
        mem[reg] = n;
    } else if (!strcmp("add", instr)) {
        mem[reg] += n;
    } else if (!strcmp("sub", instr)) {
        mem[reg] -= n;
    } else if (!strcmp("mul", instr)) {
        mem[reg] *= n;
    } else if (!strcmp("div", instr)) {
        mem[reg] /= n;
    } else if (!strcmp("mod", instr)) {
        mem[reg] = mem[reg] % n;
    } else {
        fprintf(stderr, "unknown opcode: %d: '%s'\n", line, instr);
    }
}

void reg2reg_op(char *instr, int reg1, int reg2, int line) {
    if (!strcmp("mov", instr)) {
        mem[reg1] = mem[reg2];
    } else if(!strcmp("add", instr)) {
        mem[reg1] += mem[reg2];
    } else if (!strcmp("xor", instr)) {
        mem[reg1] ^= mem[reg2];
    } else if (!strcmp("and", instr)) {
        mem[reg1] &= mem[reg2];
    }  else if (!strcmp("or", instr)) {
        mem[reg1] |= mem[reg2];
    } else {
        // fprintf(stderr, "unknown opcode: %d: '%s'\n", line, instr);
    }
}

bool isregister(char *operand) {
    assert(strlen(operand) == 1);
    return strfind(regs, operand[0]);
}

bool isnumeric(char *operand) {
    while (*operand) {
        if (!isdigit(*operand)) return false;
        operand++;
    }
    return true;
}

bool islabel(char *operand) {
    return jmptable[hash(operand)] != NIL;
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

bool includes(size_t n, char *arr[n], char *str) {
    for (int i = 0; i < n; ++i)
        if (!strcmp(str, arr[i])) return true;
    return false;
}

bool isunaryop(char *op) {
    return includes(sizeof(unaryops)/sizeof(unaryops[0]), unaryops, op);
}
bool isbinaryop(char *op) {
    return includes(sizeof(binaryops)/sizeof(binaryops[0]), binaryops, op);
}
bool isternaryop(char *op) {
    return includes(sizeof(ternaryops)/sizeof(ternaryops[0]), ternaryops, op);
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
    int linecount = 0;
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
            linecount++;
            continue;
        } else if (tok[strlen(tok)-1] == ':') {
            if (jmptable[hash(tok)] != NIL) fprintf(stderr, "duplicate label: %d: '%s'\n", lines, tok), exit(1);
            jmptable[hash(tok)] = linecount;
        }
        
        tokens[ntokens++] = strdup(tok);
        while ((tok = strtok(NULL, " "))) {
            if (!strcmp(tok, "%") || !strcmp(tok, " ")) break;
            tokens[ntokens++] = strdup(tok);
        }
        tcc += cc+1;
        linecount++;
    }

    printf("ntokens: %d\n", ntokens);

    for (int i = 0; i < ntokens; ++i) {
        if (isunaryop(tokens[i])) {
            // next token is the single param
            printf("OP: %s, PARAM: %s\n", tokens[i], tokens[i+1]);
        } else if (isbinaryop(tokens[i])) {
            // next token are the two params seperated by comma
            printf("OP: %s, PARAMs: %s\n", tokens[i], tokens[i+1]);
        } else if (isternaryop(tokens[i])) {
            // next token are the three params seperated by two commas
            printf("OP: %s, PARAMs: %s\n", tokens[i], tokens[i+1]);
        }
    }
    
    /*
    tcc = 0;
    int lp = 0;
    while (tcc < codelen) {
        int cc = 0;
        while (code[tcc+cc] != '\n') {
            cc++;
        }
        char line[cc];
        memcpy(line, code+tcc, cc);
        line[cc] = '\0';
        char *pos = NULL;
        if ((pos = strchr(line, '%'))) {    
            line[pos-line] = '\0';
        }

        char *instr = strtok(line, " ");
        char *operands = strtok(NULL, " ");
        char *op1 = strtok(operands, ",");
        char *op2 = strtok(NULL, ",");
        char *op3 = strtok(NULL, ",");
        if (!instr) {
            tcc += cc+1;
            lp++;
            continue;
        }
        if (operands && op1 && op2 && op3 && isregister(op1) && isnumeric(op2) && islabel(op3)) {
            if (!strcmp("bne", instr)) {
                if (mem[op1[strlen(op1)-1]-'a'] != strnum(op2)) {
                    tcc = jmptable[hash(op3)];
                    continue;
                }
            } else if (!strcmp("beq", instr)) {
                if (mem[op1[strlen(op1)-1]-'a'] == strnum(op2)) {
                    tcc = jmptable[hash(op3)];
                    continue;
                }
            } else if (!strcmp("blt", instr)) {
                if (mem[op1[strlen(op1)-1]-'a'] < strnum(op2)) {
                    tcc = jmptable[hash(op3)];
                    continue;
                }
            }
        } else if (operands && op1 && op2 && isregister(op1) && isnumeric(op2)) {
            int n = strnum(op2);
            num2reg_op(instr, op1[strlen(op1)-1]-'a', n, lp);
        } else if (operands && op1 && op2 && isregister(op1) && isregister(op2)) {
            reg2reg_op(instr, op1[strlen(op1)-1]-'a', op2[strlen(op2)-1]-'a', lp);
        } else if (operands && islabel(operands)) {
            // TODO: label_reg_op();
            if (!strcmp("jmp", instr)) {
                tcc = jmptable[hash(operands)];
                continue;
            } else if (!strcmp("call", instr)) {
                stack[sp] = tcc + cc + 1;
                sp++;
                tcc = jmptable[hash(operands)];
                continue;
            }
        } else if (operands && op1 && isregister(op1))  {
            // TODO: reg_op();
            if (!strcmp("#", instr)) {
                printf("%c: %d\n", op1[strlen(op1)-1], mem[op1[strlen(op1)-1]-'a']);
            }
        } else {
            if (!strcmp("ret", instr)) {
                tcc = stack[--sp];
                if (sp < 0) fprintf(stderr, "ret without corresponding call\n"), exit(1);
                continue;
            }
        }

        tcc += cc+1;
        lp++;
    }
    */
    return 0;
}
