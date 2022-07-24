// church-encoding and higher-order functions
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define len(x) (sizeof(x)/sizeof(x[0]))
typedef bool Pred(void);
bool T(void) { return true; }
bool F(void) { return false; }
Pred *True(Pred *x, Pred *y) { return x; }
Pred *False(Pred *x, Pred *y) { return y; }
Pred *Not(Pred *x) {
    return x() ? F : T;
}
Pred *And(Pred *x, Pred *y) {
    return x() ? y : F; 
}
Pred *Or(Pred *x, Pred *y) {
    return x() ? T : y; 
}
Pred *Xor(Pred *x, Pred *y) {
    return Or(And(x, Not(y)), And(Not(x), y));
}

typedef int Real(void);
int Nil(void) { return 0; }
int S(int x) { return x + 1; }
int Num(Real *x) { return x(); }

double reduce_itr(double data[], size_t len, double acc, size_t start, void f(double *, double)) {
    if (start == len) return acc;
    f(&acc, data[start]);
    return reduce_itr(data, len, acc, start+1, f);
}

double reduce(double data[], size_t len, double acc, void f(double *, double)) {
    return reduce_itr(data, len, acc, 0, f);
}

void sum(double *accum, double x) { *accum += x; }
void inc_int(void *x) { (*(int*)x)++; }
void inc_dbl(void *x) { (*(double*)x)++; }

// maps f over every element in list
// f must explicitly cast to type with compile-time known size
void map(void *list, size_t n_elems, size_t size, void f(void *)) {
    for (size_t i = 0; i != size*n_elems; i += size)
        f((void *)list+i);
}


void printarr(double arr[], size_t n_elems) {
    printf("[ ");
    for (size_t i = 0; i < n_elems; ++i)
        printf("%.2f ", arr[i]);
    printf("]\n");
}

char *P(Pred *x) { return x() ? "T" : "F"; }
int main(void) {
    double data[] = {5, 10, 15, 20, 25};  
    double res = reduce(data, len(data), 0, sum);
    map(data, len(data), sizeof(double), inc_dbl);
    
    printf("=== church-encoded logic primitives ===\n");
    printf("False(T, F) = %s\n", P(False(T, F)));
    printf("True(T, F) = %s\n", P(True(T, F)));
    printf("Not(True(T, F)) = %s\n", P(Not(True(T, F))));
    printf("And(True(T,F), True(T,F)) = %s\n", P(And(True(T, F), True(T, F))));
    printf("And(True(T,F), False(T,F)) = %s\n", P(And(True(T, F), False(T, F))));
    printf("And(False(T,F), True(T,F)) = %s\n", P(And(False(T, F), True(T, F))));
    printf("And(False(T,F), False(T,F)) = %s\n", P(And(False(T, F), False(T, F))));
    printf("Or(True(T,F), True(T,F)) = %s\n", P(Or(True(T, F), True(T, F))));
    printf("Or(True(T,F), False(T,F)) = %s\n", P(Or(True(T, F), False(T, F))));
    printf("Or(False(T,F), True(T,F)) = %s\n", P(Or(False(T, F), True(T, F))));
    printf("Or(False(T,F), False(T,F)) = %s\n", P(Or(False(T, F), False(T, F))));
    printf("Xor(True(T,F), True(T,F)) = %s\n", P(Xor(True(T, F), True(T, F))));
    printf("Xor(True(T,F), False(T,F)) = %s\n", P(Xor(True(T, F), False(T, F))));
    printf("Xor(False(T,F), True(T,F)) = %s\n", P(Xor(False(T, F), True(T, F))));
    printf("Xor(False(T,F), False(T,F)) = %s\n", P(Xor(False(T, F), False(T, F))));

    printf("\n=== church-encoded reals ===\n");
    printf("Nil() = %d\n", Nil());
    printf("S(Nil()) = %d\n", S(Nil()));
    printf("S(S(Nil())) = %d\n", S(S(Nil())));
    printf("S(S(S(Nil()))) = %d\n", S(S(S(Nil()))));
    printf("S(S(S(S(Nil())))) = %d\n", S(S(S(S(Nil())))));
    printf("S(S(S(S(S(Nil()))))) = %d\n", S(S(S(S(S(Nil()))))));
    printf("S(S(S(S(S(S(Nil())))))) = %d\n", S(S(S(S(S(S(Nil())))))));
    printf("S(S(S(S(S(S(S(Nil()))))))) = %d\n", S(S(S(S(S(S(S(Nil()))))))));
    printf("S(S(S(S(S(S(S(S(Nil())))))))) = %d\n", S(S(S(S(S(S(S(S(Nil())))))))));
    printf("S(S(S(S(S(S(S(S(S(Nil()))))))))) = %d\n", S(S(S(S(S(S(S(S(S(Nil()))))))))));
    printf("S(S(S(S(S(S(S(S(S(S(Nil())))))))))) = %d\n", S(S(S(S(S(S(S(S(S(S(Nil())))))))))));
}
