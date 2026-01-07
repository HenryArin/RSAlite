// optimization.h
#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <stdbool.h>

struct OptimizationContext {
    bool USE_SIEVE;
    bool USE_SIMD;
    bool USE_MULTITHREADING;
    bool USE_GPU;
    bool USE_BENCHMARKING;
};

#endif
