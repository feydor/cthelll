/* bfic - a brainfsck interpretor in c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXARRSIZE 30000
#define MAXCODESIZE 65536
#define PRINTSIZE 32
short int arr[MAXARRSIZE] = {0}, ptr = 0;
int stack[MAXCODESIZE], sp = 0, i = 0; // stack for matching bracket pairs
int jmptbl[MAXCODESIZE]; // save locations of matching brackets
char code[MAXCODESIZE]; int codelen, cp = 0, cin;
FILE *fp;

int main(int argc, char** argv)
{
        if (argc > 2) fprintf(stderr, "too many arguments."), exit(1);
        if (argc < 2) fprintf(stderr, "bfic needs a filename."), exit(1);
        if (!(fp = fopen(argv[1], "r"))) fprintf(stderr, "error opening the file."), exit(1);
        codelen = fread(code, 1, MAXCODESIZE, fp);
        fclose(fp);

        // first round parse for loop locations
        for (cp = 0; cp < codelen; ++cp) {
                if (code[cp] == '[') stack[sp++] = cp; // push location of '[' onto stack
                if (code[cp] == ']') {
                        if (sp == 0)
                                fprintf(stderr, "Unmatched '[' at byte %d", cp), exit(1);
                        else {
                                --sp; // found matching closing bracket, pop stack
                                jmptbl[cp] = stack[sp]; // store location of '[' at jump table for ']'
                                jmptbl[stack[sp]] = cp; // store location of ']' at jump table for '['
                        }
                }
        }
        if (sp > 0) fprintf(stderr, "Unmatched '[' at byte %d", stack[--sp]), exit(1);

        // second round parse for eval
        for (cp = 0; cp < codelen; ++cp) {
                switch(code[cp]) {
                        case '+': arr[ptr]++; break;
                        case '-': arr[ptr]--; break;
                        case '>': ptr++; break;
                        case '<': ptr--; break;
                        case '[': if (!arr[ptr]) cp = jmptbl[cp]; break;
                        case ']': if(arr[ptr]) cp = jmptbl[cp]; break;
                        case '.': putchar(arr[ptr] == 10 ? '\n' : arr[ptr]); fflush(stdout); break;
                        case ',': if ((cin = getchar()) != EOF) arr[ptr] = (cin == '\n') ? 10 : cin; break;
                        case '#': for(i=0;i<PRINTSIZE;++i) printf("%2d", (signed char)arr[i]); break;
                        default: break;
                }
        }
        return 0;
}
