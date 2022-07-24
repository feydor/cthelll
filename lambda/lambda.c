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

typedef int Real(int);
int Nil(void) { return 0; }
int S(int x) { return x + 1; }

int Compose(Real *f, int x) {
    return f(x);
}

// 1 is pos, -1 is neg, 0 is zero
int sign(int n) {
    return n == 0 ? 0 : n > 0 ? 1 : -1;
}

int ComposeN(Real *f, int x, int n) {
    if (!n) return x;
    return ComposeN(f, f(x), n-1);
}

typedef int ChurchNumeral(Real*, int, int);

int Plus(ChurchNumeral *f, int m, int n, int x) {
    return f(S, f(S, x, n), m);
}

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

    printf("\n===== church-encoded reals =====\n");
    printf("Nil() = %d\n", Nil());
    printf("Compose(S, Nil()) = %d\n", Compose(S, Nil()));
    printf("Compose(S, Compose(S, Nil())) = %d\n", Compose(S, Compose(S, Nil())));
    printf("Compose(S, Compose(S, Compose(S, Nil()))) = %d\n", Compose(S, Compose(S, Compose(S, Nil()))));
    printf("Compose(S, Compose(S, Compose(S, Compose(S, Nil())))) = %d\n", Compose(S, Compose(S, Compose(S, Compose(S, Nil())))));
    printf("Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Nil()))))) = %d\n", Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Nil()))))));

    printf("...\nusing the general definition: \n");
    printf("=== n = λf.λx.f^n x ===\n");
    printf("ComposeN(S, Nil(), 333) = %d\n", ComposeN(S, Nil(), 333));

    printf("\n===== arithmetics on church-encoded reals =====\n");
    printf("Plus(CN, C(S, C(S, Nil())), C(S, C(S, C(S, Nil()))), Nil()) = %d\n",
            Plus(ComposeN, Compose(S, Compose(S, Nil())), Compose(S, Compose(S, Compose(S, Nil()))), Nil()));
    printf("Plus(CN, CN(S, Nil(), 2), CN(S, Nil(), 3), Nil()) = %d\n", Plus(ComposeN, ComposeN(S, Nil(), 2), ComposeN(S, Nil(), 3), Nil()));
    printf("S(x) => Plus(ComposeN, 1, Nil(), Nil()) = %d\n", Plus(ComposeN, 1, Nil(), Nil()));
}
