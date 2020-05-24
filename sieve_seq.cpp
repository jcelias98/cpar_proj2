#include <iostream>
#include <iomanip>
#include <time.h>
#include <math.h>

using namespace std;

int main (int argc, char *argv[])
{
    clock_t start, end;
    double cpu_time_used;
    long long n;
    
    cout << "Power of 2: ";
    cin >> n;
 
    n = pow(2,n);
    
    start = clock();
    bool *notprimes = new bool[n];
    
    long long k = 3;

    do
    {
        //mark all odd multiples of k between k^2 and n
        for (long long j = k*k ; j<n ; j+=2*k) //fast marking
        {   notprimes[j>>1]=true;
        }
        
        do
        {
            k+=2;
        }while (k*k <= n && notprimes[k>>1]);
        
    } while (k*k <= n);
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("TIME: %lf\n", cpu_time_used);
    

    unsigned long counter = 0;
    for (int i=1; i<n; i+=2)
        if (i==1) counter=1;
        else {
            if (!notprimes[i>>1])
                counter++;
        }
    cout << counter << " ";    



}
