#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "factor.h"
#include "prime.h"

static void check_factors(int n,
                          FactorMethod method,
                          const int *expected,
                          int expected_count,
                          const char *method_name)
{
    int factors[64];
    int count = factor_number(n, method, factors, 64);

    if (count != expected_count) {
        printf("FAIL: n=%d, %s expected %d factors, got %d\n",
               n, method_name, expected_count, count);
        assert(0);
    }

    for (int i = 0; i < expected_count; i++) {
        int found = 0;
        for (int j = 0; j < count; j++) {
            if (factors[j] == expected[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("FAIL: n=%d, %s missing factor %d\n",
                   n, method_name, expected[i]);
            assert(0);
        }
    }

    printf("PASS: n=%d, %s\n", n, method_name);
}

static void run_factor_tests(void)
{
    int f2[]   = {2};
    int f3[]   = {3};
    int f4[]   = {2};
    int f6[]   = {2,3};
    int f12[]  = {2,3};
    int f55[]  = {5,11};
    int f60[]  = {2,3,5};
    int f97[]  = {97};
    int f2310[] = {2,3,5,7,11};

    FactorMethod m_trail = FACTOR_METHOD_TRIAL;
    FactorMethod m_sqrt  = FACTOR_METHOD_SQRT;
    FactorMethod m_wheel = FACTOR_METHOD_WHEEL;

    check_factors(2,    m_trail, f2,    1, "trail");
    check_factors(3,    m_trail, f3,    1, "trail");
    check_factors(4,    m_trail, f4,    1, "trail");
    check_factors(6,    m_trail, f6,    2, "trail");
    check_factors(12,   m_trail, f12,   2, "trail");
    check_factors(55,   m_trail, f55,   2, "trail");
    check_factors(60,   m_trail, f60,   3, "trail");
    check_factors(97,   m_trail, f97,   1, "trail");
    check_factors(2310, m_trail, f2310, 5, "trail");

    check_factors(2,    m_sqrt, f2,    1, "sqrt");
    check_factors(3,    m_sqrt, f3,    1, "sqrt");
    check_factors(4,    m_sqrt, f4,    1, "sqrt");
    check_factors(6,    m_sqrt, f6,    2, "sqrt");
    check_factors(12,   m_sqrt, f12,   2, "sqrt");
    check_factors(55,   m_sqrt, f55,   2, "sqrt");
    check_factors(60,   m_sqrt, f60,   3, "sqrt");
    check_factors(97,   m_sqrt, f97,   1, "sqrt");
    check_factors(2310, m_sqrt, f2310, 5, "sqrt");

    check_factors(2,    m_wheel, f2,    1, "wheel");
    check_factors(3,    m_wheel, f3,    1, "wheel");
    check_factors(4,    m_wheel, f4,    1, "wheel");
    check_factors(6,    m_wheel, f6,    2, "wheel");
    check_factors(12,   m_wheel, f12,   2, "wheel");
    check_factors(55,   m_wheel, f55,   2, "wheel");
    check_factors(60,   m_wheel, f60,   3, "wheel");
    check_factors(97,   m_wheel, f97,   1, "wheel");
    check_factors(2310, m_wheel, f2310, 5, "wheel");
}

int main(void)
{
    run_factor_tests();
    printf("All tests passed.\n");
    return 0;
}
