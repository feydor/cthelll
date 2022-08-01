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

int cumul_mutations(int *i) {
    int n = 0;
    while (!strchr(ctrl_ops, code[*i])) {
        if (code[*i] == '+') n++;
        else if (code[*i] == '-') n--;
        (*i)++;
    }
    (*i)--; // because forloop continues to next char
    return n;
}

int cumul_moves(int *i) {
    int n = 0;
    char *other_ctrl_ops = "[].,#";
    while (!strchr(mut_ops, code[*i]) && !strchr(other_ctrl_ops, code[*i])) {
        if (code[*i] == '>') n++;
        else if (code[*i] == '<') n--;
        (*i)++;
    }
    (*i)--; // because forloop continues to next char
    return n;
}

char next_op(int *i) {
    while (!strchr(ctrl_ops, code[*i]) && !strchr(mut_ops, code[*i])) (*i)++;
    return code[*i];
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
    for (int i = 0; i < codelen; ++i) {
        switch(code[i]) {
        case '+': /* FALLTHROUGH */
        case '-': printf("*ptr += %d;\n", cumul_mutations(&i)); break;
        case '>': /* FALLTHROUGH */
        case '<': printf("ptr += %d;\n", cumul_moves(&i)); break;
        case '[': {
                      int j = i+1;
                      char a = next_op(&j);
                      j++;
                      char b = next_op(&j);
                      if (a == '-' && b == ']') {
                          printf("*ptr = 0;\n");
                          i = j;
                      } else printf("while (*ptr) {\n");
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
