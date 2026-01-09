#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITIAL_CAPACITY 64

static LogEntry *logs = NULL;
static int used = 0;
static int capacity = 0;

void log_init(void)
{
    capacity = INITIAL_CAPACITY;
    used = 0;
    logs = calloc(capacity, sizeof(LogEntry));
}

void log_shutdown(void)
{
    free(logs);
    logs = NULL;
    used = capacity = 0;
}

int log_count(void)
{
    return used;
}

const LogEntry *log_get(int index)
{
    if (index < 0 || index >= used)
        return NULL;
    return &logs[index];
}

void log_clear(void)
{
    used = 0;
}

static void ensure_capacity(void)
{
    if (used < capacity)
        return;

    capacity *= 2;
    logs = realloc(logs, capacity * sizeof(LogEntry));
}

void log_add(uint64_t input,
             FactorMethod method,
             const struct OptimizationContext *opt,
             double elapsed,
             const uint64_t *factors,
             int count)
{
    ensure_capacity();

    LogEntry *e = &logs[used++];

    e->timestamp = time(NULL);
    e->input = input;
    e->method = method;
    e->use_sieve = opt ? opt->USE_SIEVE : false;
    e->benchmarking = opt ? opt->USE_BENCHMARKING : false;
    e->elapsed = elapsed;

    char *p = e->result;
    size_t left = sizeof(e->result);

    for (int i = 0; i < count; i++)
    {
        int n = snprintf(p, left, "%llu",
                         (unsigned long long)factors[i]);
        p += n;
        left -= n;

        if (i + 1 < count && left > 3)
        {
            strcpy(p, " * ");
            p += 3;
            left -= 3;
        }
    }
}

int log_export_csv(const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f)
        return -1;

    fprintf(f, "timestamp,input,method,use_sieve,benchmarking,elapsed,result\n");

    for (int i = 0; i < used; i++)
    {
        LogEntry *e = &logs[i];

        fprintf(f,
                "%ld,%llu,%d,%d,%d,%.6f,\"%s\"\n",
                (long)e->timestamp,
                (unsigned long long)e->input,
                e->method,
                e->use_sieve,
                e->benchmarking,
                e->elapsed,
                e->result);
    }

    fclose(f);
    return 0;
}
