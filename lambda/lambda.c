// church-encoding and higher-order functions
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define len(x) (sizeof(x)/sizeof(x[0]))
typedef bool Pred(void);
bool T(void) { return true; }
bool F(void) { return false; }
char *P(Pred *x) { return x() ? "T" : "F"; }
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

Pred *If(Pred *p, Pred *x, Pred *y) {
    return p() ? x : y;
}

typedef int Real(void);
typedef int RealFn(int);
int N(int x) { return x;}
int Nil(void) { return 0; }
int S(int x) { return x + 1; }

int Compose(RealFn *f, int x) {
    return f(x);
}

int ComposeN(RealFn *f, int x, int n) {
    if (!n) return x;
    return ComposeN(f, f(x), n-1);
}

typedef int ChurchNumeral(RealFn*, int, int);

int _Plus(ChurchNumeral *f, int m, int n, int x) {
    return f(S, f(S, x, n), m);
}

int Plus(int n, int m) {
    return _Plus(ComposeN, m, n, Nil());
}

int _Succ(ChurchNumeral *f, int n, int x) {
    return Compose(S, f(S, x, n));
}

int Succ(int n) {
    return _Succ(ComposeN, n, Nil());
}

Pred *IsZero(int x) {
    return Nil() == x ? T : F;
}

// typedef void Pair(int, int);
typedef struct Pair { void *first; void *second; } Pair;
Pair Cons(Real *x, Real *y) {
    Pair p = {.first = (void *)x, .second = (void *)y};
    return p;
}

Real *Car(Pair p) {
    return (Real *)p.first;
}
Real *Cdr(Pair p) {
    return (Real *)p.second;
}

void PCons(Pair p) {
    printf("( %d, %d )\n", Car(p)(), Cdr(p)());
}

int main(void) {
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
    printf("If(False(T,F), F, T) = %s\n", P(If(F, F, T)));
    printf("If(True(T,F), F, T) = %s\n", P(If(T, F, T)));

    printf("\n===== church-encoded reals =====\n");
    printf("Nil() = %d\n", Nil());
    printf("C(S, Nil()) = %d\n", Compose(S, Nil()));
    printf("C(S, C(S, Nil())) = %d\n", Compose(S, Compose(S, Nil())));
    printf("C(S, C(S, C(S, Nil()))) = %d\n", Compose(S, Compose(S, Compose(S, Nil()))));
    printf("C(S, C(S, C(S, C(S, Nil())))) = %d\n", Compose(S, Compose(S, Compose(S, Compose(S, Nil())))));
    printf("C(S, C(S, C(S, C(S, C(S, Nil()))))) = %d\n", Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Nil()))))));
    printf("C(S, C(S, C(S, C(S, C(S, C(S, Nil())))))) = %d\n", Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Nil())))))));
    printf("C(S, C(S, C(S, C(S, C(S, C(S, C(S, Nil()))))))) = %d\n", Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Nil()))))))));
    printf("C(S, C(S, C(S, C(S, C(S, C(S, C(S, C(S, Nil())))))))) = %d\n", Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Nil())))))))));
    printf("C(S, C(S, C(S, C(S, C(S, C(S, C(S, C(S, C(S, Nil()))))))))) = %d\n", Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Compose(S, Nil()))))))))));

    printf("...\nusing the general definition: \n");
    printf("=== n = λf.λx.f∘n x\n");
    printf("CN(S, Nil(), 333) = %d\n", ComposeN(S, Nil(), 333));

    printf("\n===== arithmetics on church-encoded reals =====\n");
    printf("===== plus = λm.λn.λf.λx.f∘m f∘n x\n");
    printf("Plus(CN, CN(S, Nil(), 2), CN(S, Nil(), 3), Nil()) = %d\n", _Plus(ComposeN, ComposeN(S, Nil(), 2), ComposeN(S, Nil(), 3), Nil()));
    printf("Succ(x:1) = Plus(CN, x:1, C(S, Nil()), Nil()) = %d\n", _Plus(ComposeN, 1, Compose(S, Nil()), Nil()));
    printf("Succ(CN, Nil(), CN(S, Nil(), 30)) = %d\n", _Succ(ComposeN, Nil(), ComposeN(S, Nil(), 30)));
    
    Pair p = Cons(Nil, Nil);
    PCons(p);
    //printf("Cons(33, 11) = %s\n", PCons(Cons(33, 11)));
    //printf("Car(Cons((void *)33), (void *)11) = %d\n", *(int *)Car(Cons((void *)&a, (void *)&b)));
    //printf("Cdr(Cons((void *)33), (void *)11) = %d\n", *(int *)Cdr(Cons((void *)&a, (void *)&b)));
}
