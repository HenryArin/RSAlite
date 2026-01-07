#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>

#include "factor.h"
#include "optimization.h"

static double now_seconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

static void benchmark_method(uint64_t n, FactorMethod method, const char *name)
{
    struct OptimizationContext opt = {0};
    uint64_t factors[64];

    double start = now_seconds();
    factor_number(n, method, factors, 64, &opt);
    double end = now_seconds();

    printf("%-8s : %.3f seconds\n", name, end - start);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: %s <number>\n", argv[0]);
        return 1;
    }

    uint64_t n;
    if (sscanf(argv[1], "%" SCNu64, &n) != 1 || n <= 1)
    {
        printf("invalid number\n");
        return 1;
    }

    printf("Benchmark n = %" PRIu64 "\n\n", n);

    benchmark_method(n, FACTOR_METHOD_TRIAL,   "trial");
    benchmark_method(n, FACTOR_METHOD_SQRT,    "sqrt");
    benchmark_method(n, FACTOR_METHOD_WHEEL,   "wheel");
    //benchmark_method(n, FACTOR_METHOD_FERMAT,  "fermat");
   // benchmark_method(n, FACTOR_METHOD_POLLARD, "pollard");

    return 0;
}
