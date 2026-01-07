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

	switch (method){
		case 1:
			selected_method = FACTOR_METHOD_TRAIL;
			printf("Trial Division Method selected.\n");
			break;
		case 2:
			selected_method = FACTOR_METHOD_SQRT;
			printf("Square Root Method selected.\n");
			break;
		case 3:
			selected_method = FACTOR_METHOD_WHEEL;
			printf("Wheel Factorization Method selected.\n");
			break;
		case 4:
			selected_method = FACTOR_METHOD_FERMAT;
			printf("Fermat's Method selected.\n");
			break;	
		case 5:
			selected_method = FACTOR_METHOD_POLLARD;
			printf("Pollard's Rho Method selected.\n");
			break;
		default:
			printf("Invalid method selected. Defaulting to Trial Division Method.\n");
			selected_method = FACTOR_METHOD_TRAIL;
			break;
	}

	
    
	int factors[64];
    int count = factor_number(x, selected_method, factors, 64);


    printf("Prime factors of %d \n", x);
    for (int i = 0; i < count; i++) {
        printf("%d\n", factors[i]);
    }
	return 0;
}




