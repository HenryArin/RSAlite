#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "prime.h"
#include "factor.h"
#include "optimization.h"

int main(void)
{
    struct OptimizationContext opt;
    opt.USE_SIEVE = false;
    opt.USE_SIMD = false;
    opt.USE_MULTITHREADING = false;
    opt.USE_GPU = false;

    uint64_t x;
    int method;

    printf("enter a number\n");
    scanf("%" SCNu64, &x);

    printf("enter 1 for trial division, 2 for square root, 3 for wheel, 4 for fermat, 5 for pollard\n");
    scanf("%d", &method);

    FactorMethod selected_method;

    switch (method)
    {
    case 1:
        selected_method = FACTOR_METHOD_TRIAL;
        break;
    case 2:
        selected_method = FACTOR_METHOD_SQRT;
        break;
    case 3:
        selected_method = FACTOR_METHOD_WHEEL;
        break;
    case 4:
        selected_method = FACTOR_METHOD_FERMAT;
        break;
    case 5:
        selected_method = FACTOR_METHOD_POLLARD;
        break;
    default:
        selected_method = FACTOR_METHOD_TRIAL;
        break;
    }

    int sieve_choice;

    printf("Enable Sieve: press 1; Otherwise press 0\n");
    scanf("%d", &sieve_choice);

    opt.USE_SIEVE = (sieve_choice == 1);

    uint64_t factors[64];
    int count = factor_number(x, selected_method, factors, 64, &opt);

    printf("Prime factors of %" PRIu64 ":\n", x);
    for (int i = 0; i < count; i++)
    {
        printf("%" PRIu64 "\n", factors[i]);
    }

    return 0;
}
