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
Pred *Not(Pred *x) { return x() ? F : T; }

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

// TODO: varargs, printf like func
void printarr(double arr[], size_t n_elems) {
    printf("[ ");
    for (size_t i = 0; i < n_elems; ++i)
        printf("%.2f ", arr[i]);
    printf("]\n");
}

int main(void) {
    double data[] = {5, 10, 15, 20, 25};  
    double res = reduce(data, len(data), 0, sum);
    printf("result: %f\n", res);
    
    printarr(data, len(data));
    map(data, len(data), sizeof(double), inc_dbl);
    printarr(data, len(data));
    
    printf("False(T, F) = %s\n", False(T, F) ? "T" : "F");
    printf("True(T, F) = %s\n", True(T, F) ? "T" : "F");
    printf("Not(True(T, F)) = %s\n", Not(True(T, F)) ? "T" : "F");
}
