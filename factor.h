// FACTOR_H
#include <stdbool.h>
#include "optimization.h"
#ifndef FACTOR_H
#define FACTOR_H


typedef enum {
    FACTOR_METHOD_TRIAL,
    FACTOR_METHOD_SQRT,
    FACTOR_METHOD_WHEEL,
    FACTOR_METHOD_FERMAT,
    FACTOR_METHOD_POLLARD,

} FactorMethod;


//void get_factors(int x);
int factor_number(int n,
                  FactorMethod method,
                  int *factors,
                  int max_factors,
                  const struct OptimizationContext *opt);
int factor_with_trial(int n, int *factors, int max_factors, bool use_sieve);
int factor_with_sqrt(int n, int *factors, int max_factors);
int factor_with_wheel(int n, int *factors, int max_factors);
int factor_with_sieve(int n, int *factors, int max_factors);
int factor_with_fermat(int n, int *factors, int max_factors);
int factor_with_pollard(int n, int *factors, int max_factors);




#endif 