//main.c
#include <stdio.h>
#include <stdlib.h>
#include "prime.h"
#include "factor.h"


int main() {
	int x;
	int method;
	printf("enter a number\n");
	scanf("%d",&x);

	printf("enter 1 for trail division method enter 2 for square root method\n");
	scanf("%d",&method);
	if (method == 1) {
    int factors[64];
    int count = factor_with_sqrt(x, factors, 64);

    printf("Prime factors of %d (trial method, using sqrt for now):\n", x);
    for (int i = 0; i < count; i++) {
        printf("%d\n", factors[i]);
    }
	} else if (method == 2) {
		int factors[64];
    int count = factor_with_sqrt(x, factors, 64);

    printf("Prime factors of %d (sqrt method):\n", x);
    for (int i = 0; i < count; i++) {
        printf("%d\n", factors[i]);
    }
	} else {
		return 1;
	}
	return 0;

}




