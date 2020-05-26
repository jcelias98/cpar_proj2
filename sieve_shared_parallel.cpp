#include <iostream>
#include <iomanip>
#include <math.h>
#include <omp.h>

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
    
    start = omp_get_wtime();
    #pragma omp parallel num_threads(THREADS) private(j)
    do
    {
        #pragma omp parallel for 
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

    unsigned long counter = 0;
    for (long long int i=1; i<n; i+=2)
        if (i==1) counter=1;
        else {
            if (!notprimes[i>>1])
                counter++;
        }
    cout << counter << "\n";  

}
