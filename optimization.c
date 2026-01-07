

#include <stdio.h>
#include <stdlib.h>

int use_sieve() {
    int input;
    printf("enter a number\n");
    scanf("%d",&input);
    
    
    
    int *prime = malloc((input + 1) * sizeof(int));

    for( int i = 2; i * i <= input; i++){
        if(prime[i] == 0){
            for( int j = i * i; j <= input; j = j + i){
                prime[j] = 1;
            
           
        }
        

    }
    
    

}
for (int i = 2; i <= input; i++){
    if (prime[i] == 0){
        printf("%d\n", i);
    }
    
}    free(prime);

}

