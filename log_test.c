#include "log.h"
#include <stdio.h>

int main(void)
{
    log_init();

    uint64_t factors[] = {2, 3, 5};
    uint64_t f97[] = {97};

    log_add(30,
            FACTOR_METHOD_TRIAL,
            NULL,
            0.00123,
            factors,
            3);

    log_add(97,
            FACTOR_METHOD_SQRT,
            NULL,
            0.00004,
            f97, 1);

    printf("Log count: %d\n", log_count());

    log_export_csv("test.csv");

    log_shutdown();

    printf("CSV written to test.csv\n");
    return 0;
}
