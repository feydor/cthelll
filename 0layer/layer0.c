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
            
void eval_unary(size_t op_idx, char *param, size_t *curr_tok) {
    char *op = unaryops[op_idx];
    if (!strcmp("jmp", op)) {
        *curr_tok = jmptable[hash(param) + ':']; // NOTE: adding back in the label character
    } else if (!strcmp("#", op)) {
        printf("%s: %d\n", param, mem[param[0]-'a']);
    } else {
        printf("unary operator, %s, not yet implemented\n", op);
    }
}

// NOTE: clobbers params string
void eval_binary(size_t op_idx, char *params) {
    // l and rparams can be: reg,reg or reg,num
    char *lparam = strtok(params, ",");
    char *rparam = strtok(NULL, ",");
    if (!lparam || !rparam) {
        fprintf(stderr, "Parse error in binary op"), exit(-1);
    } else if (!isregister(lparam)) {
        fprintf(stderr, "Left param in a binary op must be a register: '%s'", lparam), exit(-1);
    }

    size_t lreg = lparam[0]-'a';
    char *op = binaryops[op_idx];
    if (!strcmp("mov", op)) {
        if (isnumeric(rparam)) fprintf(stderr, "mov params must both be registers\n"), exit(-1);
        mem[lreg] = mem[rparam[0]-'a'];
    } else if (!strcmp("add", op)) {
        if (isnumeric(rparam)) {
            mem[lreg] += strnum(rparam);
        } else {
            size_t rreg = rparam[0]-'a';
            mem[lreg] += mem[rreg];
        }        
    } else if (!strcmp("sub", op)) {
        if (isnumeric(rparam)) {
            mem[lreg] -= strnum(rparam);
        } else {
            size_t rreg = rparam[0]-'a';
            mem[lreg] -= mem[rreg];
        }
    } else if (!strcmp("xor", op)) {
        if (isnumeric(rparam)) {
            mem[lreg] ^= strnum(rparam);
        } else {
            size_t rreg = rparam[0]-'a';
            mem[lreg] ^= mem[rreg];
        }
    } else if (!strcmp("mul", op)) {
        if (isnumeric(rparam)) {
            mem[lreg] *= strnum(rparam);
        } else {
            size_t rreg = rparam[0]-'a';
            mem[lreg] *= mem[rreg];
        }
    } else if (!strcmp("mod", op)) {
        if (isnumeric(rparam)) {
            mem[lreg] %= strnum(rparam);
        } else {
            size_t rreg = rparam[0]-'a';
            mem[lreg] %= mem[rreg];
        }
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

    printf("ntokens: %d\n", ntokens);

    for (size_t i = 0; i < ntokens; ++i) {
        size_t op = -1;
        if ((op = unaryop(tokens[i])) != -1) {
            // next token is the single param
            printf("OP: %s, PARAM: %s\n", unaryops[op], tokens[i+1]);
            eval_unary(op, tokens[i+1], &i);
            i++;
        } else if ((op = binaryop(tokens[i])) != -1) {
            // next token are the two params seperated by comma
            printf("OP: %s, PARAMs: %s\n", binaryops[op], tokens[i+1]);
            eval_binary(op, tokens[i+1]);
            i++;
        } else if ((op = ternaryop(tokens[i])) != -1) {
            // next token are the three params seperated by two commas
            printf("OP: %s, PARAMs: %s\n", ternaryops[op], tokens[i+1]);
            // eval_ternary(op, tokens[i+1], &i);
            i++;
        }
    }
    
    return 0;
}
