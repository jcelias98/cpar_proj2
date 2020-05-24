#include <iostream>
#include <iomanip>
#include <time.h>
#include <math.h>
#include <omp.h>

#define THREADS 8
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
        //#pragma omp parallel for schedule (static, 3) num_threads(THREADS)
        # pragma omp parallel for schedule (static, 8)
        //mark all odd multiples of k between k^2 and n
        for (j = k*k ; j<n ; j+=2*k) //fast marking
        {   notprimes[j>>1]=true;
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
    

    /*unsigned long counter = 0;
    for (int i=1; i<n; i+=2) 
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

    unsigned long long counter = 0;
    for (long long int i=1; i<n; i+=2)
        if (i==1) counter=1;
        else {
            if (!notprimes[i>>1])
                counter++;
        }
    cout << counter << "\n";    

}
