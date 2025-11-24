/*The purpose of factor.c is: given an integer x, 
compute and either print or store its prime factors,
using primality checks from prime.c*/
#include <stdio.h>
#include "factor.h"
#include "prime.h"
#include <limits.h>

/*
int get_factors(int x, void (*callback)(int)) {
    for ( int j = 1; j <= x; j++) {
        if (x % j == 0) {
            callback(j);
        }
}
}
*/


int factor_with_sqrt(int n, int *factors, int max_factors)
{
    int count = 0;

    for (int j = 1; j <= n; j++) {
        if (n % j == 0) {
            if (is_prime_sqrt(j)) {
                if (count < max_factors) {
                    factors[count++] = j;
                }
            }
        }
    }

    return count;
}

int factor_with_trail(int n, int *factors, int max_factors)
{
    int count = 0;

    for (int j = 1; j <= n; j++) {
        if (n % j == 0) {
            if (is_prime_trail(j)) {
                if (count < max_factors) {
                    factors[count++] = j;
                }
            }
        }
    }

    return count;
}


int factor_number(int n, FactorMethod method, int *factors, int max_factors)
{
    switch (method) {
        case FACTOR_METHOD_TRAIL:
            return factor_with_trail(n, factors, max_factors);

        case FACTOR_METHOD_SQRT:
            return factor_with_sqrt(n, factors, max_factors);

        // TODO: add Pollard Rho here later
        // case FACTOR_METHOD_POLLARD:
        //     return factor_with_pollard(n, factors, max_factors);

        default:
            return 0;
    }
}
