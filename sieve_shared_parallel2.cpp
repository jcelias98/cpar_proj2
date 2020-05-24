#include <iostream>
#include <iomanip>
#include <time.h>
#include <math.h>
#include <omp.h>

//Blocks division Macros
#define BLOCK_LOW(i,p,n) ((i)*(n)/(p))
#define BLOCK_HIGH(i,p,n) (BLOCK_LOW((i)+1,p,n)-1)
#define BLOCK_SIZE(i,p,n) (BLOCK_LOW(i+1,p,n)-BLOCK_LOW(i,p,n))
#define BLOCK_OWNER(index,p,n) (((p)*(index)+1)-1)/(n)

using namespace std;

int main (int argc, char *argv[])
{
    clock_t start, end;
    double cpu_time_used;
    long long n;
    
    cout << "Power of 2: ";
    cin >> n;
 
    n = pow(2,n);
    
    bool *notprimes = new bool[n];
    
    long long k = 3;
    long long j;
    
    start = omp_get_wtime();
    #pragma omp parallel private(j)
    do
    {
        #pragma omp parallel for 
        //mark all odd multiples of k between k^2 and n
        for (j = BLOCK_LOW(omp_get_thread_num(), omp_get_num_threads(), n)>>1; j < BLOCK_SIZE(omp_get_thread_num(), omp_get_num_threads(), n)>>1 ; j+=2*k) //fast marking
        {   
            if (j >= k*k) notprimes[j>>1]=true;
        }
        #pragma omp master
        do
        {
            k+=2;
        }while (k*k <= n && notprimes[k>>1]);
        #pragma omp barrier
    } while (k*k <= n);
    
    end = omp_get_wtime();
    cpu_time_used = ((double) (end - start));
    printf("TIME: %lf\n", cpu_time_used);
    

    unsigned long counter = 0;
    for (long long int i=1; i<n; i+=2) 
        if (i==1) {
            counter=1;
            cout << 2 << " "; 
        }

        else {
            if (!notprimes[i>>1]) {
                counter++;
                cout << i << " ";
            }

        }
    printf("\n");
    cout << counter << "\n";    

}
