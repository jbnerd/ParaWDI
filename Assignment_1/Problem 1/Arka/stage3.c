#include <stdio.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>
#define UPPER_BOUND 300
#define MODE 1 // 0 to force entire task in one process

int main(int argc, char* argv[]);

void
timestamp()
{
#define TIME_SIZE 40
  static char time_buffer[TIME_SIZE];
  const struct tm* tm;
  size_t len;
  time_t now;
  now = time(NULL);
  tm = localtime(&now);
  len = strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);
  return;
#undef TIME_SIZE
}

int
main(int argc, char* argv[])
{
  int id, p, root = 0;
  long n, i;
  long ptr;
  long n_hi, n_lo = 1, limit;
  long prime, dprime, start, count = 0, total_count;
  double wtime;

  // setting limit
  char** end;
  // setting limit
  if (argc > 0 && MODE == 1)
    limit = strtol(argv[1], end, 10);
  else
    limit = UPPER_BOUND; /*setting upper bound */
  long lsqrt = (long)ceil(sqrt(limit));

  /*  Initialize MPI. */
  int ierr = MPI_Init(NULL, NULL);
  /*  Get the number of processes. */
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &p);
  /*  Determine this processes's rank. */
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
    char* arr = (char*)calloc((n_hi - n_lo), sizeof(char));

    // find next prime
    ptr = n_lo;
    prime = 3;
    while (prime <= n_hi && prime > 1) { // get next prime
      if (id == root) {
        while (arr[ptr - n_lo] == 1) {
          ptr += 1;
          if (ptr > lsqrt) // can comment out maybe
          {
            ptr = limit + 1;
            break;
          }
        }
        prime = ptr;
        ptr += 1;
      }

      // broadcast prime
      // int er_a = prime;
      ierr = MPI_Bcast(&prime, 1, MPI_INT, root, MPI_COMM_WORLD);
      if (prime >= n_hi)
        break;

      // eliminate multiples of prime in n_lo to n_hi
      start = (long)ceil((n_lo * 1.0) / prime) * prime;
      if (id == root)
        start += prime;
      dprime = 2 * prime;
      if (prime == 2) {
        for (i = start - n_lo; i < n_hi - n_lo; i += prime)
          arr[i] = 1;
      } else if (start % 2 == 0) {
        start += prime;
        for (i = start - n_lo; i < n_hi - n_lo; i += dprime)
          arr[i] = 1;
      } else {
        for (i = start - n_lo; i < n_hi - n_lo; i += dprime)
          arr[i] = 1;
      }
    }

    int *rcounts, *dsply;
    if (id == root) {
      rcounts = (int*)calloc(p, sizeof(int));
      dsply = (int*)calloc(p, sizeof(int));
    }

    MPI_Gather(&count, 1, MPI_INT, rcounts, 1, MPI_INT, root, MPI_COMM_WORLD);

    if (id == root) {
      for (i = 0; i < p; i++) {
        dsply[i] = total_count;
        total_count += rcounts[i];
        printf("Process %d : %d  ..  %d \n", i, rcounts[i], dsply[i]);
      }
    }

    long k = 0;
    long subdomain_size;
    if (id == p - 1)
      subdomain_size = limit - n_lo;
    else
      subdomain_size = n_hi - n_lo;

    long* primes = (long*)malloc(count * sizeof(long));
    for (i = 0; i < subdomain_size; i++) {
      if (arr[i] != 1) {
        primes[k] = i + n_lo;
        k++;
      }
    }

    long* all_primes;
    if (id == root)
      all_primes = (long*)calloc(total_count, sizeof(long));

    MPI_Gatherv(primes, count, MPI_LONG, all_primes, rcounts, dsply, MPI_LONG,
                root, MPI_COMM_WORLD);

    // Uncomment the following code to print all the primes.
    for (i = 0; i < total_count; i++) {
      // printf("%d\n", all_primes[i]);
    }

    if (id == root) {
      wtime = MPI_Wtime() - wtime;
      printf("         N        Pi          Time\n");
      printf("  %10ld %10ld  %16f\n", limit, total_count, wtime);
    }
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
      char* arr = (char*)calloc((limit - 1), sizeof(char));
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
      printf("  %8d  %8d  %14f\n", limit, total_count, wtime);
      // free(arr);
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
