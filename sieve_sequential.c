#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sieve(unsigned long n)
{
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    //Init
    char* primes = (char*) malloc(n+1);
    char finish = 0;
    unsigned long i, k, j;
    for(i = 0; i < n+1; i++) primes[i] = 1;

    //Process
    for(k = 2; (k*k) <= n && finish == 0; ) {
        for(j = k*k; j<=n; j++) {
            if(j%k == 0) primes[j] = 0; 
        }
        finish = 1;
        for (i = k+1; i < n+1; i++) {
            if(primes[i] == 1) {
                finish = 0;
                k = i;
                break;
            }
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    //Out
    unsigned long c = 0;
    for(int i = 2; i < n+1; i++) {
        if(primes[i] == 1) {
            c++;
        }
    }
    printf("PRIMES: %lu\n", c);
    printf("TIME: %lf\n", cpu_time_used);
}

int main(){
    unsigned long n;
    scanf("%lu", &n);

    sieve(n);
    return 0;
}