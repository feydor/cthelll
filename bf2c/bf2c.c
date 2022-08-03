#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCODESIZE 3333
#define MEMSIZE 30000
#define PRINTSIZE 32
char code[MAXCODESIZE];
int codelen;
char *ctrl_ops = "><[].,#";
char *mut_ops = "+-";

typedef struct BfOp {
    int index;
    int operand;
} BfOp;

int cumul_mutations(int *cp) {
    int n = 0;
    while (!strchr(ctrl_ops, code[*cp])) {
        if (code[*cp] == '+') n++;
        else if (code[*cp] == '-') n--;
        (*cp)++;
    }
    return n;
}

int cumul_moves(int *cp) {
    int n = 0;
    char *other_ctrl_ops = "[].,#";
    while (!strchr(mut_ops, code[*cp]) && !strchr(other_ctrl_ops, code[*cp])) {
        if (code[*cp] == '>') n++;
        else if (code[*cp] == '<') n--;
        (*cp)++;
    }
    return n;
}

char next_op(int *cp) {
    while (!strchr(ctrl_ops, code[*cp]) && !strchr(mut_ops, code[*cp])) (*cp)++;
    return code[*cp];
}

bool is_mutating(int cp) {
    return code[cp] == '+' || code[cp] == '-';
}

bool is_moving(int cp) {
    return code[cp] == '>' || code[cp] == '<';
}

void fun() {
    /**
     * a loop consists of:
     * 1. the last ptr location before the loop starts
     * 2. all of the ops inside of the loop
     *      - ops == consecutive moves or mutations
     *        ">> ----" are two ops:
     *              - Op1{type: MOV, addr: memptr, operand: 2}, Op2{type: MUT, addr: memptr+2, operand: -4}
     *      - these can be combined into mutations at specific addresses
     *              - Op{type: MUT, addr: memptr+Op1.operand, operand: -4}
     * 3. if one of the ops addr == addr before loop starts, then that op is being used as a loop index
     *      - ie while (cells[IndexOp.addr]-->0)
     *    then the rest of the combined Ops can be considered multiply ops of IndexOp.original_memory * Op1.operand
     */
}

int main(int argc, const char *argv[]) {
    if (argc > 2) fprintf(stderr, "too many arguments\n"), exit(1);
    if (argc < 2) fprintf(stderr, "bf2c needs a filename\n"), exit(1);
    FILE *fp = NULL;
    if (!(fp = fopen(argv[1], "r"))) fprintf(stderr, "error opening file\n"), exit(1);
    codelen = fread(code, 1, MAXCODESIZE, fp);
    fclose(fp);

    printf("#include <stdio.h>\nint main(void){\n");
    printf("short int cells[%d] = {0};\nshort int *ptr = cells;\n", MEMSIZE);
    printf("int stack[%d], sp = 0;\n", MAXCODESIZE);
    printf("char cin;\n");
    for (int cp = 0; cp < codelen; ++cp) {
        switch(code[cp]) {
        case '+': /* FALLTHROUGH */
        case '-': printf("*ptr += %d;\n", cumul_mutations(&cp)); --cp; break;
        case '>': /* FALLTHROUGH */
        case '<': printf("ptr += %d;\n", cumul_moves(&cp)); --cp; break;
        case '[': {
                      int j = cp+1;
                      char a = next_op(&j);
                      j++;
                      char b = next_op(&j);
                      if (a == '-' && b == ']') {
                          printf("*ptr = 0;\n");
                          cp = j;
                      }

                      else printf("while (*ptr) {\n");
                  } break;
        case ']': printf("}\n"); break;
        case '.': printf("putchar(*ptr == 10 ? '\\n' : *ptr); fflush(stdout);\n"); break;
        case ',': printf("if ((cin = getchar()) != EOF) *ptr = (cin == '\n') ? 10 : cin;\n"); break;
        case '#': printf("for(int i=0;i<32;++i) printf(\"%%2d\", (signed char)*ptr);\n"); break;
        default: break;
        }
    }
    
    printf("return 0;\n}\n");
    return 0;
}
