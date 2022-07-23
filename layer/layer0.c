#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCODESIZE 333
#define NON_NUMERIC INT_MAX
#define NIL -1
FILE *fp;
char code[MAXCODESIZE];
int jmptable[MAXCODESIZE];
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
void num2reg_op(char *instr, char reg, int n, int line) {
    if (!strcmp("mov", instr)) {
        mem[reg-97] = n;
    } else if (!strcmp("add", instr)) {
        mem[reg-97] += n;
    } else if (!strcmp("sub", instr)) {
        mem[reg-97] -= n;
    } else {
        fprintf(stderr, "unknown opcode: %d\n", line);
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
int main(int argc, char *argv[]) {
    if (argc != 2) printf("program: layer0 [file]\n"), exit(1);
    if (!(fp = fopen(argv[1], "r"))) fprintf(stderr, "error opening the file.\n"), exit(1);
    codelen = fread(code, 1, MAXCODESIZE, fp);
    fclose(fp);
    for (int i = 0; i < sizeof(jmptable)/sizeof(jmptable[0]); ++i)
        jmptable[i] = NIL;
    // first pass
    int tcc = 0;
    lines = 1;
    while (tcc != codelen) {
        int cc = 0;
        while (code[tcc+cc] != '\n') {
            cc++;
        }
        char line[cc];
        memcpy(line, code+tcc, cc);
        line[cc] = '\0';
        char *instr = strtok(line, " ");
        if (line[strlen(line)-1] == ':') {
            char *lbl = strtok(line, ":");
            if (jmptable[hash(lbl)] != NIL) fprintf(stderr, "duplicate label: %d\n", lines), exit(1);
            jmptable[hash(lbl)] = tcc;
        } else if (!strcmp("jmp", instr)) {
            char *cmd = strtok(NULL, " ");
            if (!cmd) fprintf(stderr, "missing label: %s\n", line), exit(1);
            if (jmptable[hash(cmd)] == NIL) fprintf(stderr, "invalid label: %d: %s\n", lines, cmd), exit(1);
        }
        tcc += cc+1;
        lines++;
    }
    
    tcc = 0;
    int lp = 1;
    while (tcc < codelen) {
        int cc = 0;
        while (code[tcc+cc] != '\n') {
            cc++;
        }
        char line[cc];
        memcpy(line, code+tcc, cc);
        line[cc] = '\0';
        char *instr = strtok(line, " ");
        char *operands = strtok(NULL, " ");
        char *op1 = strtok(operands, ",");
        char *op2 = strtok(NULL, ",");
        char *op3 = strtok(NULL, ",");
        if (!instr) break;
        
        if (operands && op1 && op2 && op3 && isregister(op1) && isnumeric(op2) && islabel(op3)) {
            if (!strcmp("bne", instr)) {
                int n = strnum(op2);
                if (mem[op1[0]-97] != n) {
                    tcc = jmptable[hash(op3)];
                    continue;
                }
            }
        } else if (operands && op1 && op2 && isregister(op1) && isnumeric(op2)) {
            int n = strnum(op2);
            num2reg_op(instr, op1[0], n, lp);
        } else if (operands && islabel(operands)) {
            // TODO: label_reg_op();
            if (!strcmp("jmp", instr)) {
                tcc = jmptable[hash(operands)];
                continue;
            }
        } else if (operands && op1 && isregister(op1))  {
            // TODO: reg_op();
            if (!strcmp("#", instr)) {
                printf("%c: %d\n", op1[0], mem[op1[0]-97]);
            }
        }

        tcc += cc+1;
        lp++;
    }
    return 0;
}
