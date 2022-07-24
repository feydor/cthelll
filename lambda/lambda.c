// church-encoding, maybe?
#include <stdio.h>
#include <stdbool.h>
#define len(x) (sizeof(x)/sizeof(x[0]))
bool T(void) { return true; }
bool F(void) { return false; }
void sum(double *accum, double x) { *accum += x; }
double reduce_itr(double data[], size_t len, double acc, size_t start, void f(double *, double)) {
    if (start == len) return acc;
    f(&acc, data[start]);
    return reduce_itr(data, len, acc, start+1, f);
}

double reduce(double data[], size_t len, double acc, void f(double *, double)) {
    return reduce_itr(data, len, acc, 0, f);
}

bool True(bool (x(void)), bool (y(void))) {
    return x();
}

bool False(bool (x(void)), bool (y(void))) {
    return y();
}

bool Not(bool (x(void))) {
    return x() ? F() : T();    
}

int main(void) {
    double data[] = {5, 10, 15, 20, 25};
    double res = reduce(data, len(data), 0, sum);
    printf("result: %f\n", res);
    
    bool r1 = False(T, F);
    bool r2 = True(T, F);
    printf("False(T, F) = %s\n", r1 ? "T" : "F");
    printf("True(T, F) = %s\n", r2 ? "T" : "F");
    printf("Not(F) = %s\n", Not(F) ? "T" : "F");
}
