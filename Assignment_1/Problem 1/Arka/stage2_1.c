// Solution by Arka Talukdar

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>
#define UPPER_BOUND 9
#define MODE 0  // 0 to forsake CL-ARG
#define PRINT 0 // 1 to print primes.

int main(int argc, char *argv[]);

//returns more than o

int get(unsigned long* vector, long pos){
    int m,d,h;
    d = pos/64;
    m = pos%64;
    k = (1<<m);
    if (vec[d] & k) 
        return 1;
    return 0;
}

void set(unsigned long* vec, long pos){
    int m,d,h;
    d = pos/64;
    m = pos%64;
    k = (1<<m);
    vec[d] = ( vec[d] | k )
}

int reset(unsigned long* vec, long pos){
    int m,d,h;
    d = pos/64;
    m = pos%64;
    k = (1<<m);
    vec[d] = ( vec[d] & (! k) );
}

void timestamp() {
#define TIME_SIZE 40
  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t len;
  time_t now;
  now = time(NULL);
  tm = localtime(&now);
  len = strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);
  return;
#undef TIME_SIZE
}

int main(int argc, char *argv[]) {
  int id, p, root = 0;
  long n, i, dprime;
  long ptr;
  long n_hi, n_lo = 1, limit;
  long prime, start, count = 0, total_count = 0;
  double wtime;
  char **end;
  // setting limit
  if (argc > 0 && MODE == 1)
    limit = (long)pow(10,atoi(argv[1]));
  else
    limit = (long)pow(10,UPPER_BOUND); /*setting upper bound */
  long lsqrt = (long)ceil(sqrt(limit));

  /*  Initialize MPI. */
  int ierr = MPI_Init(NULL, NULL);
  /*  Get the number of processes.*/
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &p);
  /*  Determine this processes's rank.*/
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);
  //-------Start of timer -------
  if (id == 0)
    wtime = MPI_Wtime();
  // ---- handling larger cases ----
  if (lsqrt < (long)ceil(limit / p * 1.0)) {
    if (id == 0) {
      timestamp();
      printf("\n");
      printf("PRIME_MPI\n");
      printf("  C/MPI version\n");
      printf("\n");
      printf("  An MPI example program to count the number of primes.\n");
      printf("  The number of processes is %d", p);
      printf("\n");
    }
    // ------- create arrays ------
    if (id != p - 1)
      n_hi = (long)(limit / p) * (id + 1);
    else
      n_hi = limit;
    if (id == root)
      n_lo = 2;
    else
      n_lo = (long)(limit / p) * id;

    char *arr = (char *)calloc((n_hi - n_lo), sizeof(char));
    char *sarr = (char *)calloc((lsqrt + 1), sizeof(char));

    ptr = n_lo;
    prime = 2;

    while (prime <= lsqrt && prime > 1) { // get next prime
      while (prime <= lsqrt && get( sarr,prime ) == 1 )
        prime += 1;
      if (prime > lsqrt)
        break;

      for (i = 2 * prime; i <= lsqrt; i += prime)
        set(sarr,i);

      // eliminate multiples of prime in n_lo to n_hi
      start = prime * prime;
      //printf("%ld \n", start);
      dprime = 2 * prime;
      if (prime == 2) {
        for (i = start - n_lo; i < n_hi - n_lo; i += prime){
          arr[i] = 1; 
        }

      } else if (start % 2 == 0) {
        start += prime;
        for (i = start - n_lo; i < n_hi - n_lo; i += dprime)
          set(sarr,i);
      } else {
        for (i = start - n_lo; i < n_hi - n_lo; i += dprime)
          set(sarr,i);
      }
      prime += 1;
    }
printf("hello\n");
    // count primes
    if (id != p - 1) {
      for (i = 0; i < n_hi - n_lo; i += 1)
        count += 1 - get(arr,i);
    } else {
      for (i = 0; i < limit - n_lo; i += 1)
        count += 1 - get(arr,i);
    }
    // ierr = MPI_Reduce ( &count, &total_count, 1, MPI_LONG, MPI_SUM, root ,
    // MPI_COMM_WORLD );

    int *rcounts, *dsply;
    if (id == root) {
      rcounts = (int *)calloc(p, sizeof(int));
      dsply = (int *)calloc(p, sizeof(int));
    }

    MPI_Gather(&count, 1, MPI_INT, rcounts, 1, MPI_INT, root, MPI_COMM_WORLD);

    if (id == root) {
      for (i = 0; i < p; i++) {
        dsply[i] = total_count;
        total_count += rcounts[i];
        //printf("Process %ld : %d  ..  %d \n", i, rcounts[i], dsply[i]);
      }
    }

    long k = 0;
    long subdomain_size;
    if (id == p - 1)
      subdomain_size = limit - n_lo;
    else
      subdomain_size = n_hi - n_lo;

    long *primes = (long *)malloc(count * sizeof(long));
    for (i = 0; i < subdomain_size; i++) {
      if (get(arr,i) == 0) {
        primes[k] = i + n_lo;
        k++;
      }
    }

    long *all_primes;
    if (id == root)
      all_primes = (long *)calloc(total_count, sizeof(long));

    MPI_Gatherv(primes, count, MPI_LONG, all_primes, rcounts, dsply, MPI_LONG,
                root, MPI_COMM_WORLD);
    // Uncomment the following code to print all the primes.
    if(MODE == 1 ){
    	for (i = 0; i < total_count; i++) {
      		// printf("%d\n", all_primes[i]);    
	}
    }
    if (id == root) {
      wtime = MPI_Wtime() - wtime;
      printf("         N        Pi          Time\n");
      printf("  %10ld %10ld  %16f\n", limit, total_count, wtime);
    }
    //  free(arr);
  } else {
    if (id == 0) {
      timestamp();
      printf("\n");
      printf("PRIME_MPI\n");
      printf("  C/MPI version\n");
      printf("\n");
      printf("  An MPI example program to count the number of primes.\n");
      printf(
          "  The number of processes is %d but using only root process due to "
          "small input.",
          p);
      printf("\n");
      char *arr = (char *)calloc((limit - 1), sizeof(char));
      ptr = 2;
      while (ptr <= lsqrt) {
        if (arr[ptr] == 0) {
          for (i = 2 * ptr - 2; i <= limit - 2; i += ptr)
            arr[i] = 1;
        }
        ptr += 1;
      }
      printf(" Marked C\n");
      for (i = 0; i < limit - 2; i++)
        count += arr[i];
      wtime = MPI_Wtime() - wtime;
      printf("         N        Pi          Time\n");
      printf("  %10ld %10ld  %16f\n", limit, total_count, wtime);
      //  free(arr);
    }
  }
  /*   Terminate. Code for performance measurement */
  if (id == 0) {
    printf("\n");
    printf("PRIME_MPI - Master process:\n");
    printf("  Normal end of execution.\n");
    printf("\n");
    timestamp();
  }
  /*   Terminate MPI. */
  ierr = MPI_Finalize();
  return 0;
}
