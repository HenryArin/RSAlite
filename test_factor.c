#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>

#include "factor.h"
#include "prime.h"

#define MAX_FACTORS 64
#define MAX_N 10000
#define PROGRESS_STEP 1000

static int is_prime_ref(uint64_t n)
{
    if (n < 2)
        return 0;

    for (uint64_t i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

static int reference_factors(uint64_t n, uint64_t *out, int max)
{
    int count = 0;

    for (uint64_t p = 2; p <= n; p++)
    {
        if (!is_prime_ref(p))
            continue;

        if (n % p == 0)
        {
            if (count >= max)
            {
                fprintf(stderr,
                        "Reference factor overflow for n=%" PRIu64 "\n",
                        n);
                assert(0);
            }
            out[count++] = p;
        }
    }
    return count;
}

static void compare_factors(uint64_t n,
                            const uint64_t *expected, int expected_count,
                            const uint64_t *actual, int actual_count,
                            const char *method_name)
{
    if (expected_count != actual_count)
    {
        printf("FAIL: n=%" PRIu64 ", %s expected %d factors, got %d\n",
               n, method_name, expected_count, actual_count);
        assert(0);
    }

    for (int i = 0; i < expected_count; i++)
    {
        int found = 0;
        for (int j = 0; j < actual_count; j++)
        {
            if (expected[i] == actual[j])
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            printf("FAIL: n=%" PRIu64 ", %s missing factor %" PRIu64 "\n",
                   n, method_name, expected[i]);
            assert(0);
        }
    }
}

static void test_number(uint64_t n, FactorMethod method, const char *method_name)
{
    uint64_t expected[MAX_FACTORS];
    uint64_t actual[MAX_FACTORS];

    int expected_count = reference_factors(n, expected, MAX_FACTORS);

    printf("calling factor_number: n=%" PRIu64 " method=%s\n",
           n, method_name);
    fflush(stdout);

    int actual_count = factor_number(n, method, actual, MAX_FACTORS, NULL);

    compare_factors(n,
                    expected, expected_count,
                    actual, actual_count,
                    method_name);
}

static void run_all_tests(void)
{
    FactorMethod methods[] = {
        FACTOR_METHOD_TRIAL,
        FACTOR_METHOD_SQRT,
        FACTOR_METHOD_WHEEL};

    const char *names[] = {
        "trial",
        "sqrt",
        "wheel"};

    const int method_count = sizeof(methods) / sizeof(methods[0]);

    for (int m = 0; m < method_count; m++)
    {
        printf("Testing %s...\n", names[m]);

        for (uint64_t n = 2; n <= MAX_N; n++)
        {
            test_number(n, methods[m], names[m]);

            if (n % PROGRESS_STEP == 0)
            {
                printf("  %s: reached n=%" PRIu64 "\n",
                       names[m], n);
            }
        }

        printf("Completed %s\n\n", names[m]);
    }
}

int main(void)
{
    run_all_tests();
    printf("All factor tests passed for n = 2..%d\n", MAX_N);
    return 0;
}
