//main.c
#include <stdio.h>
#include <stdlib.h>
#include "prime.h"



int main() {
	int x;
	int method;
	printf("enter a number\n");
	scanf("%d",&x);

	printf("enter 1 for trail division method enter 2 for square root method\n");
	scanf("%d",&method);
	if(method == 1) {
		trail_division(x);
	} else if (method == 2) {
		square_root(x);
	} else {
		return 1;
	}
	return 0;

}




