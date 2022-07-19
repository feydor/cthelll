#include <stdio.h>
char buf[255];
char sp = 0;

int main(void) {
    char c;
    for (;;) {
        while ((c = getchar()) != '\n') {
            buf[sp] = c;
            sp++;
        }
        while (sp > -1) {
            putc(buf[sp], stdout);
            sp--;
        }
        sp = 0;
        putc('\n', stdout);
    }
    return 0;
}