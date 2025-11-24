// FACTOR_H
#ifndef FACTOR_H
#define FACTOR_H


typedef enum {
    FACTOR_METHOD_TRAIL,
    FACTOR_METHOD_SQRT
    // Add more here later: FACTOR_METHOD_POLLARD, etc.
} FactorMethod;


//void get_factors(int x);
int factor_with_sqrt(int n, int *factors, int max_factors);
int factor_with_trail(int n, int *factors, int max_factors);
int factor_number(int n, FactorMethod method, int *factors, int max_factors);



#endif 