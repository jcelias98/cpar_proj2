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
    int THREADS = omp_get_num_procs();

    double start, end;
    double cpu_time_used;
    long long n;

    int a;
    cout << "Num. of Threads: ";
    cin >> a;

    if(a) THREADS = a;
    
    cout << "Power of 2: ";
    cin >> n;
 
    n = pow(2,n);
    
    bool *notprimes = new bool[n];
    
    long long k = 3;
    long long j;
    int low, high, t;
    
    start = omp_get_wtime();
    #pragma omp parallel num_threads(THREADS) private(j, low, high, t)
    do
    {
        //mark all odd multiples of k between k^2 and n
        //int low = 
        //low = (low%2) == 0 ? (low+1)>>1 : low >> 1;
        //int high = BLOCK_HIGH(omp_get_thread_num(), omp_get_num_threads(), n);
        //high = (high%2) == 0 ? (high+1)>>1 : high >> 1;
        //cout<< omp_get_thread_num() << " - " << BLOCK_LOW(omp_get_thread_num(), omp_get_num_threads(), n) << " - " << BLOCK_HIGH(omp_get_thread_num(), omp_get_num_threads(), n)<<endl;
        low = BLOCK_LOW(omp_get_thread_num(), omp_get_num_threads(), n);
        high = BLOCK_HIGH(omp_get_thread_num(), omp_get_num_threads(), n);

        if(high < k*k) low = high + 1;
        else if ( high == k*k) low = high;
        else if ( low <= k*k) low = k*k;
        else {
            t = (low-k*k)/(2*k);
            if((low-k*k)%(2*k)) t++;
            low = k*k + t*2*k;
        }
        
        for (j = low; j <= high; j+=2*k) //fast marking
        {   
            notprimes[j>>1]=true;
        }
        #pragma omp barrier
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
        if (i==1) counter=1;
        else {
            if (!notprimes[i>>1])
                counter++;
        }
    cout << counter << "\n";  

    /*unsigned long counter = 0;
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
    */
}
