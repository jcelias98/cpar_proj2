#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


#define BLOCK_LOW(i,p,n) ((i)*(n-2)/(p) )
#define BLOCK_HIGH(i,p,n) (BLOCK_LOW((i)+1,p,n) -1)
#define BLOCK_SIZE(i,p,n) (BLOCK_LOW(i+1,p,n)-BLOCK_LOW(i,p,n))
#define BLOCK_OWNER(index,p,n) (((p)*(index)+1)-1)/(n)


int main(int argc, char* argv[]) {

    int p; // number of processes
    int id; // id of the current process
    unsigned long n; // 2^n

    unsigned long first_elem;
    unsigned long last_elem;
    unsigned long size;
    unsigned int counter;
    unsigned int global_counter;

    unsigned long j;
    unsigned long k;

    unsigned long index_first_multiple;
    unsigned long next_k;

    MPI_Init(&argc, &argv); // Initialize the MPI
    MPI_Comm_size(MPI_COMM_WORLD, &p); //  number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &id); // id of the process

    double start = MPI_Wtime();
    double time;

    if(!(argc == 2)){
        if(id == 1) printf("Error: please enter a parameter n to find primes less than 2^n");
        MPI_Finalize();
        exit(1);
    }

    n = atol(argv[1]);
    n = pow(2,n);

    //  n/p > sqrt(n) to ensure the next value of k is always in processor 0
    if(!(n/p > sqrt(n))){
        if(id == 1) printf("Too many processes! \n");
        MPI_Finalize();
        exit (1);
    }

    // The array is split on blocks of equal size with first and last elements of each processor
    first_elem = BLOCK_LOW(id, p, n) + 2;
    last_elem = BLOCK_HIGH(id, p, n) + 2;
    size = BLOCK_SIZE(id, p, n );

    
    bool *primes = new bool[size]; // each processor has its own array of size "size"
    for(j = 0; j < size; j++){
        primes[j] = true; // all elements are marked as prime
    }

    // initially k=2 for each process
    k = 2;

    while(k*k <= n){
        
        // compute the index of the first multiple of k in the current vector of the thread
        index_first_multiple;
        if(first_elem % k == 0) index_first_multiple = 0;
        else index_first_multiple = k - first_elem % k;

   
        // mark all multiples of k as non-prime in the current thread's vector
        for(j = index_first_multiple; j < size; j += k){ //fast marking
            primes[j] = false;
        }

        if(id == 0){ //parent process 
               // the element at index 0 of processor 0 is 2   
                primes[k-2] = true; //   k is a prime number that we need to mark in the  array of processor 0
        }

        // Processor 0 is responsable to find the next k and broadcast it to the other processors 
        // This value is in processor 0 because  n/p > sqrt(n)
        if(id == 0){ //parent process
            next_k = k+1;
            while(!primes[next_k - 2]) next_k = next_k + 1;
            k = next_k; 
        }

        // we need to broadcast k to the other threads
        MPI_Bcast (&k,  1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    counter= 0;
    global_counter;
    for(j = 0; j < size; j++){
        if (primes[j]) counter++;
    }
    MPI_Reduce (&counter, &global_counter, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(id == 0){
        time = MPI_Wtime() - start;
        printf(" TIME: %f \n %d \n", time, global_counter);
    }
   
    delete(primes);
    MPI_Finalize();

}
    				