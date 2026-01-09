#ifndef FACTOR_H
#define FACTOR_H

#include <stdbool.h>
#include <stdint.h>
#include "optimization.h"

typedef enum
{
    FACTOR_METHOD_TRIAL,
    FACTOR_METHOD_SQRT,
    FACTOR_METHOD_WHEEL,
    FACTOR_METHOD_FERMAT,
    FACTOR_METHOD_POLLARD
} FactorMethod;

int factor_number(uint64_t n,
                  FactorMethod method,
                  uint64_t *factors,
                  int max_factors,
                  const struct OptimizationContext *opt);

int factor_with_trial(uint64_t n,
                      uint64_t *factors,
                      int max_factors,
                      bool use_sieve,
                      const struct OptimizationContext *opt);

int factor_with_sqrt(uint64_t n,
                     uint64_t *factors,
                     int max_factors,
                     const struct OptimizationContext *opt);

int factor_with_wheel(uint64_t n,
                      uint64_t *factors,
                      int max_factors,
                      const struct OptimizationContext *opt);

int factor_with_fermat(uint64_t n,
                       uint64_t *factors,
                       int max_factors,
                       const struct OptimizationContext *opt);

int factor_with_pollard(uint64_t n,
                        uint64_t *factors,
                        int max_factors,
                        const struct OptimizationContext *opt);

#endif
