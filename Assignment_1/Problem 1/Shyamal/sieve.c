// @author = 53 68 79 61 6D 61 6C 
// date	  = 13/02/2018

# include <stdio.h>
# include <time.h>
# include <math.h>
# include <mpi.h>
# include <stdlib.h>
# include <string.h>


void decompose_domain(int domain_size, int world_rank, int world_size, int* subdomain_start, int* subdomain_size) {
    /*******************************************************************
    * DESCRIPTION : Assign the subdomain to given process
    * NOTES:     
    *           [1] Argument domain_size: Size of the over all domain (In this case value of N)
    *
    */
    
    *subdomain_start = domain_size / world_size * world_rank;
    *subdomain_size = domain_size / world_size;
    if (world_rank == 0){
        *subdomain_start = 2;
        *subdomain_size -= 1;
    }
    if (world_rank == world_size - 1) {
        // Give remainder to last process
        *subdomain_size += domain_size % world_size;
    }
}

void mark_composites(char* arr, int subdomain_start, int subdomain_size, int prime, int world_rank){
    /*******************************************************************
    * DESCRIPTION : Mark the Composites in the given arr of the perticular process
    * NOTES:     
    *           [1] Need to be carefule about root process as it starts from 2 and not 0
    *
    */
    int i, index;
    if(world_rank == 0){
        i = 2 * prime - subdomain_start;
    }else{
        i = ceil((float)subdomain_start / prime)*prime;
        i = (int) i - subdomain_start;
    }
    for(; i < subdomain_size; i+= prime){
        arr[i] = 1;
    }
}

void next_prime(char* arr, int curr_prime, int* new_prime, int limit){
    /*******************************************************************
    * DESCRIPTION : Find the next prime number in the array
    * NOTES:     
    *           [1] Should only be run in root process
    *           [2] Returns -1 on excedding the limit (sqrt(N)); Will be useful in terminating processes
    *
    */
    int i = curr_prime - 2 + 1;
    *new_prime = -1;
    while(arr[i] == 1 && i < limit ) i++;
    if ( i < limit ) *new_prime = i + 2;
}

int main(int argc, char** argv){
    int domain_size;
    
    // handle having less than required arguments;
    if (argc > 0)
        domain_size = (int)atoi(argv[1]);
    else
        domain_size = 1000000;

    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int limit = (int)ceil(sqrt(domain_size));
    int root = 0;
    int subdomain_start, subdomain_size;
    double wtime;

    if (world_rank == root)
        wtime = MPI_Wtime();

    decompose_domain(domain_size, world_rank, world_size, &subdomain_start, &subdomain_size);
    
    

    char *arr = (char*) malloc((subdomain_size)* sizeof(char));
    memset(arr, 0, subdomain_size*sizeof(char));
    // printf("Arrays Created with start: %d and size: %d\n", subdomain_start, subdomain_size);

    int curr_prime, prime = 1;
    while(1){
        if(world_rank == 0){
            curr_prime = prime;
            next_prime(arr, curr_prime, &prime, limit);
            // printf("Current Prime is %d, and in process %d\n", prime, world_rank);        
        }
        MPI_Bcast(&prime, 1, MPI_INT, root, MPI_COMM_WORLD);
        if(prime == -1) break;        
        mark_composites(arr, subdomain_start, subdomain_size, prime, world_rank);
    }
    // printf("Composite marked!\n");
    
    int i, count = 0, total_count = 0;
    for(i = 0; i < subdomain_size; i ++){
        if(arr[i] != 1){
            count += 1;
        }
    }

    int *rcounts, *dsply;
    if (world_rank == root){
        rcounts = (int*) malloc(world_size * sizeof(int));
        dsply = (int*) malloc(world_size * sizeof(int));
    }

    MPI_Gather(&count, 1, MPI_INT, rcounts, 1, MPI_INT, root, MPI_COMM_WORLD);

    int *primes = (int*) malloc(count * sizeof(int));
    int k = 0;
    for(i = 0; i < subdomain_size; i ++){
        if(arr[i] != 1){
            primes[k] = i + subdomain_start;
            k++;
        }
    }

    if(world_rank == root){
        for(i=0; i < world_size; i++){
            dsply[i] = total_count;            
            total_count += rcounts[i];
        }
    }

    int* all_primes;
    if (world_rank == root)
        all_primes = (int*) malloc(total_count * sizeof(int));
    
    MPI_Gatherv(primes, count, MPI_INT, all_primes, rcounts, dsply, MPI_INT, root, MPI_COMM_WORLD);
    
    // Uncomment the following code to print all the primes. 
    
    // for(i = 0; i < total_count; i++){
    //     printf("%d\n", all_primes[i]);
    // }
    
    //free(arr);
    
    if(world_rank == root){
        wtime = MPI_Wtime() - wtime;
        printf("Total count: %d, Total Time taken: %f\n", total_count, wtime);
    }
    MPI_Finalize();
}