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

    for (int j = 1; j * j <= n; j++) {
        if (n % j == 0) {
            if (is_prime_sqrt(j)) {
                if (count < max_factors) {
                    factors[count++] = j;
                }
            }
            int k = n / j;
            if (k != j && is_prime_sqrt(k)) {
                if (count < max_factors) {
                    factors[count++] = k;
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

int factor_with_wheel(int n, int *factors, int max_factors)
{
    int count = 0;
    if (n <= 1) {
        return 0;
    }

    int small_primes[] = {2, 3, 5};
    for (int i = 0; i < 3; i++) {
        int p = small_primes[i];
        if (n % p == 0) {
            if (count < max_factors) {
                factors[count++] = p;
            }
            while (n % p == 0) {
                n /= p;
            }
        }
    }

    if (n == 1) {
        return count;
    }

    int residues[] = {1, 7, 11, 13, 17, 19, 23, 29};
    int base = 30;
    int k = 0;

    for (;;) {
        for (int i = 0; i < 8; i++) {
            int d = base * k + residues[i];
            if (d <= 1) {
                continue;
            }

            if (d > n / d) {
                goto done;
            }

            if (n % d == 0) {
                if (count < max_factors) {
                    factors[count++] = d;
                }

                while (n % d == 0) {
                    n /= d;
                }

                if (n == 1) {
                    return count;
                }
            }
        }
        k++;
    }

done:
    if (n > 1 && is_prime_wheel(n)) {
        if (count < max_factors) {
            factors[count++] = n;
        }
    }
    return count;
}

int factor_with_sieve(int n, int factors[], int max_factors)
{
    return factor_with_trail(n, factors, max_factors);
}

int factor_with_fermat(int n, int factors[], int max_factors)
{
    return factor_with_sqrt(n, factors, max_factors);
}

int factor_with_pollard(int n, int factors[], int max_factors)
{
    return factor_with_sqrt(n, factors, max_factors);
}

int factor_number(int n, FactorMethod method, int *factors, int max_factors)
{
    switch (method) {
        case FACTOR_METHOD_TRAIL:
            printf("DEBUG: using Trial Division\n");
            return factor_with_trail(n, factors, max_factors);

        case FACTOR_METHOD_SQRT:
            printf("DEBUG: using Square Root\n");
            return factor_with_sqrt(n, factors, max_factors);

        case FACTOR_METHOD_WHEEL:
            printf("DEBUG: using Wheel factorization\n");
            return factor_with_wheel(n, factors, max_factors);
        
        case FACTOR_METHOD_SIEVE:
            printf("DEBUG: Sieve method not implemented\n");
            return factor_with_sieve(n, factors, max_factors);

        case FACTOR_METHOD_FERMAT:
            printf("DEBUG: Fermat method not implemented\n");
            return factor_with_fermat(n, factors, max_factors);

        case FACTOR_METHOD_POLLARD:
            printf("DEBUG: Pollard method not implemented\n");
            return factor_with_pollard(n, factors, max_factors);

        default:
            printf("DEBUG: unknown method\n");
            return 0;
    }
}


