#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 

//Blocks division Macros
#define BLOCK_LOW(i,p,n) ((i)*(n)/(p))
#define BLOCK_HIGH(i,p,n) (BLOCK_LOW((i)+1,p,n)-1)
#define BLOCK_SIZE(i,p,n) (BLOCK_LOW(i+1,p,n)-BLOCK_LOW(i,p,n))
#define BLOCK_OWNER(index,p,n) (((p)*(index)+1)-1)/(n))


void sieve(unsigned long n)
{
    clock_t start, end;
    double cpu_time_used;

    char* primes = (char*) malloc(n+1);
    char finish = 0;
    unsigned long i, k, j;

    int procs = omp_get_num_procs();
    if((n+1) < procs) omp_set_num_threads(n+1);

    start = omp_get_wtime();
    #pragma omp parallel private(i,j)
    {
        #pragma omp parallel for
        for(i = BLOCK_LOW(omp_get_thread_num(), omp_get_num_threads(), n+1); i < BLOCK_SIZE(omp_get_thread_num(), omp_get_num_threads(), n+1); i++) {
            primes[i] = 1;
        }
        k = 2;

        while((k*k) <= n && finish == 0) {
            #pragma omp parallel for
            for(j = BLOCK_LOW(omp_get_thread_num(), omp_get_num_threads(), n+1); j < BLOCK_SIZE(omp_get_thread_num(), omp_get_num_threads(), n+1); j++) {
                if(j%k == 0 && j >= k*k) primes[j] = 0; 
            }
            #pragma omp master
            {
                finish = 1;
                for (i = k+1; i < n+1; i++) {
                    if(primes[i] == 1) {
                        finish = 0;
                        k = i;
                        break;
                    }
                }
            }
            #pragma omp barrier
        }
    }
    end = omp_get_wtime();
    cpu_time_used = ((double) (end - start));

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