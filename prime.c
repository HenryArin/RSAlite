//prime.c
#include <stdio.h>
#include <stdlib.h>
#include "prime.h"



void trail_division(int x) {
	struct finding_primes *s1 = malloc(sizeof(struct finding_primes));
	if (s1 == NULL) {
		printf("Memory allocation failed.\n");
		return;
	}
	for ( int j = 1; j <= x; j++) {
			if ( x % j == 0) {
			// setting temp to j for later, need to maniuplate j without messing it up
			s1->temp = j;
			// assume it's prime
			s1->isPrime = 1;
			if (s1->temp == 1) {
				s1->isPrime = 0;
			} else {
				for ( int i = 2; i <= s1->temp/2; i++) {
					if(s1->temp % i == 0) {
						s1->isPrime = 0;
						break;
					}
				}
				if (s1->isPrime) {
					printf("%d is a prime factor of %d\n", j, x);
				}
			}
	}
	}

	
	free(s1);
}


void square_root(int x) {
	struct finding_primes *s2 = malloc(sizeof(struct finding_primes));
	if(s2 == NULL) {
		printf("Memory allocation failed.\n");
		return;
	}
	for ( int j = 1; j <= x; j++) {
		//find the factors
		if ( x % j == 0) {
			// setting temp to j for later, need to maniuplate j without messing it up
			s2->temp = j;
			// assume it's prime
			s2->isPrime = 1;
			if (s2->temp == 1) {
				s2->isPrime = 0;
			} else {
				for ( int i = 2; i * i <= s2->temp ; i++) {
					if(s2->temp % i == 0) {
						s2->isPrime = 0;
						break;
					}
				}
				if (s2->isPrime) {
					printf("%d is a prime factor of %d\n", j, x);
				}
			}
		}

	}
	free(s2);
}

