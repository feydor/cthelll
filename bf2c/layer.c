#include <stdio.h>
#include <stdlib.h>
#define put(c)(putc(c, stdout))
char mem[255] = {0};
char buf[255];
char mp = 0, ip = 0, lp = 0;

int main(void) {
    char c;
    for (;;) {
        while ((c = getchar()) != '\n') buf[ip++] = c;
        switch (buf[0]) {
        case '>': while (buf[ip--] == '>') mp++; break;
        case '<': mp = mp-1 < 0 ? 0 : mp-1; break;
        case '.': {
            if (ip < 2) exit(-1);
            while (--ip) mem[mp++] = buf[ip];
        } break;
        case '[': lp = buf[0]; break;
        case ']': mp = lp; break;
        case '#': put(mem[mp]); break;
        case 'q': return 0;
        }
        ip = 0;
        c = ' ';
    }
    return 0;
}
