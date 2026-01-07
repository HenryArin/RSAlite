#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "factor.h"
#include "prime.h"
#include "optimization.h"


int factor_with_trial(int n, int *factors, int max_factors, bool use_sieve)
{
    int count = 0;

    if (n <= 1)
        return 0;

    int *prime = NULL;

    if (use_sieve)
    {
        prime = generate_prime_with_sieve(n);
        if (!prime)
            return 0;
    }

    for (int p = 2; p * p <= n; p++)
    {
        if (use_sieve)
        {
            if (prime[p] == 1)
                continue;
        }
        else
        {
            if (!is_prime_trail(p))
                continue;
        }

        if (n % p == 0)
        {
            if (count < max_factors)
                factors[count++] = p;

            while (n % p == 0)
                n /= p;
        }
    }

    if (n > 1 && count < max_factors)
        factors[count++] = n;

    free(prime);

    return count;
}

int factor_with_sqrt(int n, int *factors, int max_factors)
{
    int count = 0;

    for (int p = 2; p * p <= n; p++)
    {
        if (is_prime_sqrt(p) && n % p == 0)
        {
            if (count < max_factors)
                factors[count++] = p;

            while (n % p == 0)
                n /= p;
        }
    }

    if (n > 1 && count < max_factors)
        factors[count++] = n;

    return count;
}

int factor_with_wheel(int n, int *factors, int max_factors)
{
    int count = 0;

    if (n <= 1)
        return 0;

    int small_primes[] = {2, 3, 5};
    for (int i = 0; i < 3; i++)
    {
        int p = small_primes[i];
        if (n % p == 0)
        {
            if (count < max_factors)
                factors[count++] = p;

            while (n % p == 0)
                n /= p;
        }
    }

    if (n == 1)
        return count;

    int residues[] = {1, 7, 11, 13, 17, 19, 23, 29};
    int base = 30;

    for (int k = 0;; k++)
    {
        for (int i = 0; i < 8; i++)
        {
            int d = base * k + residues[i];
            if (d <= 1)
                continue;

            if (d > n / d)
                goto done;

            if (n % d == 0)
            {
                if (count < max_factors)
                    factors[count++] = d;

                while (n % d == 0)
                    n /= d;
            }
        }
    }

done:
    if (n > 1 && count < max_factors)
        factors[count++] = n;

    return count;
}

int factor_with_fermat(int n, int factors[], int max_factors)
{
    return factor_with_sqrt(n, factors, max_factors);
}

int factor_with_pollard(int n, int factors[], int max_factors)
{
    return factor_with_sqrt(n, factors, max_factors);
}

int factor_number(int n,
                  FactorMethod method,
                  int *factors,
                  int max_factors,
                  const struct OptimizationContext *opt)
{
    switch (method)
    {
    case FACTOR_METHOD_TRIAL:
        return factor_with_trial(n, factors, max_factors, opt->USE_SIEVE);

    case FACTOR_METHOD_SQRT:
        return factor_with_sqrt(n, factors, max_factors);

    case FACTOR_METHOD_WHEEL:
        return factor_with_wheel(n, factors, max_factors);

    case FACTOR_METHOD_FERMAT:
        return factor_with_fermat(n, factors, max_factors);

    case FACTOR_METHOD_POLLARD:
        return factor_with_pollard(n, factors, max_factors);

    default:
        return 0;
    }
}
