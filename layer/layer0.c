#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCODESIZE 333
#define NON_NUMERIC INT_MAX
FILE *fp;
char code[MAXCODESIZE];
int jmptable[MAXCODESIZE] = {-1};
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
int num2reg_operand(char* op1, char *num, int line) {
    if (!op1 || !num) fprintf(stderr, "invalid operands: %d\n", line), exit(1);
    char reg = op1[0];
    if (!strfind(regs, reg)) fprintf(stderr, "invalid register: %d: %c\n", line, reg), exit(1);
    int n = strnum(num);
    if (n == NON_NUMERIC) fprintf(stderr, "non-numeric operand: %d: %s\n", line, num), exit(1);
    return n;
}
int main(int argc, char *argv[]) {
    if (argc != 2) printf("program: layer0 [file]\n"), exit(1);
    if (!(fp = fopen(argv[1], "r"))) fprintf(stderr, "error opening the file.\n"), exit(1);
    codelen = fread(code, 1, MAXCODESIZE, fp);
    fclose(fp);
    // first pass
    int tcc = 0;
    lines = 1;
    while (tcc != codelen) {
        int cc = 0;
        while (code[tcc+cc] != '\n') {
            printf("'%c'", code[tcc+cc]);
            cc++;
        }
        printf("\n");
        char line[cc];
        memcpy(line, code+tcc, cc);
        line[cc] = '\0';
        char *instr = strtok(line, " ");
        if (line[strlen(line)-1] == ':') {
            char *lbl = strtok(line, ":");
            if (jmptable[hash(lbl)]) fprintf(stderr, "duplicate label: %d\n", lines), exit(1);
            jmptable[hash(lbl)] = tcc;
        } else if (!strcmp("jmp", instr)) {
            char *cmd = strtok(NULL, " ");
            if (!cmd) fprintf(stderr, "missing label: %s\n", line), exit(1);
            // TODO: what if hash collsions aka duplicate labels
            if (jmptable[hash(cmd)] < 0) fprintf(stderr, "invalid label: %d: %s\n", lines, cmd), exit(1);
        }
        tcc += cc+1;
        lines++;
    }
    printf("pass 1 done\n\n");
    
    tcc = 0;
    int lp = 1;
    while (tcc != codelen) {
        int cc = 0;
        while (code[tcc+cc] != '\n') {
            cc++;
        }
        char line[cc];
        memcpy(line, code+tcc, cc);
        line[cc] = '\0';
        char *instr = strtok(line, " ");
        if (!strcmp("mov", instr)) {
            char *cmd = strtok(NULL, " ");
            if (!cmd) fprintf(stderr, "missing operands: %d\n", lp), exit(1);
            char *op1 = strtok(cmd, ",");
            char *op2 = strtok(NULL, ",");
            mem[op1[0]-97] = num2reg_operand(op1, op2, lp);
        } else if (!strcmp("#", instr)) {
            char *op1 = strtok(NULL, " ");
            if (!op1) fprintf(stderr, "invalid operand: %d\n", lp), exit(1);
            if (!strfind(regs, op1[0])) fprintf(stderr, "invalid register: %d: %c\n", lp, op1[0]), exit(1);
            printf("%c: %d\n", op1[0], mem[op1[0]-97]);
        } else if (!strcmp("jmp", instr)) {
            char *cmd = strtok(NULL, " ");
            tcc = jmptable[hash(cmd)];
            continue;
        }
        tcc += cc+1;
        lp++;
    }
    return 0;
}
