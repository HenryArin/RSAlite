#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "factor.h"
#include "prime.h"
#include "optimization.h"

int factor_with_trial(uint64_t n, uint64_t *factors, int max_factors, bool use_sieve)
{
    int count = 0;

    if (n <= 1)
        return 0;

    if (use_sieve && n > 10000000ULL)
        use_sieve = false;

    int *prime = NULL;

    if (use_sieve)
    {
        prime = generate_prime_with_sieve((int)n);
        if (!prime)
            return 0;
    }

    for (uint64_t p = 2; p <= n / p; p++)
    {
        
        if (use_sieve)
        {
            if (prime[p])
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

int factor_with_sqrt(uint64_t n, uint64_t *factors, int max_factors)
{
    int count = 0;

    for (uint64_t p = 2; p <= n / p; p++)
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

int factor_with_wheel(uint64_t n, uint64_t *factors, int max_factors)
{
    int count = 0;

    if (n <= 1)
        return 0;

    uint64_t small_primes[] = {2, 3, 5};

    for (int i = 0; i < 3; i++)
    {
        uint64_t p = small_primes[i];
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

    for (uint64_t k = 0;; k++)
    {
        for (int i = 0; i < 8; i++)
        {
            uint64_t d = base * k + residues[i];

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

int factor_with_fermat(uint64_t n, uint64_t *factors, int max_factors)
{
    return factor_with_sqrt(n, factors, max_factors);
}

int factor_with_pollard(uint64_t n, uint64_t *factors, int max_factors)
{
    return factor_with_sqrt(n, factors, max_factors);
}

int factor_number(uint64_t n,
                  FactorMethod method,
                  uint64_t *factors,
                  int max_factors,
                  const struct OptimizationContext *opt)
{
    static const struct OptimizationContext default_opt = {
        .USE_SIEVE = 0
    };

    if (opt == NULL)
        opt = &default_opt;

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
