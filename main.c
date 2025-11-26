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

	printf("enter 1 for trail division method enter 2 for square root method enter 3 for wheel\n");
	scanf("%d",&method);

	FactorMethod selected_method;

	if (method == 1) {
		selected_method = FACTOR_METHOD_TRAIL;
	} else if (method == 2) {
		selected_method = FACTOR_METHOD_SQRT;
		} else if (method == 3) {
		selected_method = FACTOR_METHOD_WHEEL;
	} else {
		printf("Invalid method selected.\n");
		return 1;
	}
    
	int factors[64];
    int count = factor_number(x, selected_method, factors, 64);


    printf("Prime factors of %d \n", x);
    for (int i = 0; i < count; i++) {
        printf("%d\n", factors[i]);
    }
	return 0;
}




