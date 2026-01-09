#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static LogEntry *entries = NULL;
static int entry_count = 0;
static int entry_capacity = 0;

void log_init(void)
{
    entry_capacity = 64;
    entry_count = 0;
    entries = malloc(sizeof(LogEntry) * entry_capacity);
}

static void ensure_capacity(void)
{
    if (entry_count < entry_capacity)
        return;

    entry_capacity *= 2;
    entries = realloc(entries, sizeof(LogEntry) * entry_capacity);
}

void log_add(uint64_t input,
             FactorMethod method,
             const struct OptimizationContext *opt,
             double elapsed,
             const uint64_t *factors,
             int count)
{
    ensure_capacity();

    LogEntry *e = &entries[entry_count++];
    e->timestamp = time(NULL);
    e->input = input;
    e->method = method;
    e->use_sieve = opt ? opt->USE_SIEVE : false;
    e->benchmarking = opt ? opt->USE_BENCHMARKING : false;
    e->elapsed = elapsed;

    char *p = e->result;
    size_t left = sizeof(e->result);

    for (int i = 0; i < count && left > 1; i++)
    {
        int written = snprintf(p, left, "%llu%s",
            (unsigned long long)factors[i],
            (i < count - 1) ? "×" : "");

        if (written < 0 || (size_t)written >= left)
            break;

        p += written;
        left -= written;
    }
}

int log_count(void)
{
    return entry_count;
}

const LogEntry *log_get(int index)
{
    if (index < 0 || index >= entry_count)
        return NULL;
    return &entries[index];
}

void log_clear(void)
{
    entry_count = 0;
}

void log_shutdown(void)
{
    free(entries);
    entries = NULL;
    entry_count = 0;
    entry_capacity = 0;
}
