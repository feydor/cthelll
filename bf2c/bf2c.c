#include <stdio.h>
#include <stdlib.h>
#define MAXCODESIZE 3333
#define MEMSIZE 30000
#define PRINTSIZE 32
char code[MAXCODESIZE];

int main(int argc, const char *argv[]) {
    if (argc > 2) fprintf(stderr, "too many arguments\n"), exit(1);
    if (argc < 2) fprintf(stderr, "bf2c needs a filename\n"), exit(1);
    FILE *fp = NULL;
    if (!(fp = fopen(argv[1], "r"))) fprintf(stderr, "error opening file\n"), exit(1);
    int codelen = fread(code, 1, MAXCODESIZE, fp);
    fclose(fp);

    printf("#include <stdio.h>\nint main(void){\n");
    printf("short int cells[%d] = {0}, ptr = 0;\n", MEMSIZE);
    printf("int stack[%d], sp = 0;\n", MAXCODESIZE);
    printf("char cin;\n");
    for (int i = 0; i < codelen; ++i) {
        switch(code[i]) {
        case '+': printf("cells[ptr]++;\n"); break;
        case '-': printf("cells[ptr]--;\n"); break;
        case '>': printf("ptr++;\n"); break;
        case '<': printf("ptr--;\n"); break;
        case '[': {
                      printf("while (cells[ptr]) {\n");
                  } break;
        case ']': {
                      printf("}\n");
                  } break;
        case '.': printf("putchar(cells[ptr] == 10 ? '\\n' : cells[ptr]); fflush(stdout);\n"); break;
        case ',': printf("if ((cin = getchar()) != EOF) cells[ptr] = (cin == '\n') ? 10 : cin;\n"); break;
        case '#': printf("for(int i=0;i<32;++i) printf(\"%%2d\", (signed char)cells[i]);\n"); break;
        default: break;
        }
    }
    
    printf("return 0;\n}\n");
    return 0;
}