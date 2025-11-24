//prime.c
#include <stdio.h>
#include <stdlib.h>
#include "prime.h"


int is_prime_sqrt(int n)
{
    if (n == 1) {
        return 0;
    }

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int is_prime_trail(int n)
{
    if (n == 1) {
        return 0;
    }

    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}
