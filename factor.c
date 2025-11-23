#include <stdio.h>
#include "factor.h"


void get_factors(int x, void (*callback)(int)) {
    for ( int j = 1; j <= x; j++) {
        if (x % j == 0) {
            callback(j);
        }
}
}

