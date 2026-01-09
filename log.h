#ifndef RSALITE_LOG_H
#define RSALITE_LOG_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "factor.h"      
#include "optimization.h"

typedef struct
{
    time_t timestamp;
    uint64_t input;
    FactorMethod method;
    bool use_sieve;
    bool benchmarking;
    double elapsed;
    char result[256];
} LogEntry;

void log_init(void);
void log_add(uint64_t input,
             FactorMethod method,
             const struct OptimizationContext *opt,
             double elapsed,
             const uint64_t *factors,
             int count);

int log_count(void);
const LogEntry *log_get(int index);
void log_clear(void);
void log_shutdown(void);

int log_export_csv(const char *path);

#endif
