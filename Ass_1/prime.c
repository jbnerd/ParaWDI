# include <stdio.h>
# include <time.h>
# include <math.h>
# include <mpi.h>
# include <stdlib.h>

#define UPPER_BOUND  9999999

int main ( int argc, char *argv[] );

void timestamp ()
{
# define TIME_SIZE 40
  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t len;
  time_t now;
  now = time( NULL );
  tm = localtime( &now );
  len = strftime( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );
  printf( "%s\n", time_buffer );
  return;
# undef TIME_SIZE
}

int main ( int argc, char *argv[] )
{
  int i, id, ierr;
  int n;
  int ptr;
  int n_hi, n_lo, limit;
  int p, prime, start, count, total_count;
  double wtime;

  n_lo = 1;
  
  if (argc > 0)
    limit = (int)atoi(argv[1]);
  else
    limit = UPPER_BOUND;     /*setting upper bound */  
    
/*  Initialize MPI. */
  ierr = MPI_Init ( NULL, NULL );
/*  Get the number of processes.*/
  ierr = MPI_Comm_size ( MPI_COMM_WORLD, &p );
/*  Determine this processes's rank.*/
  ierr = MPI_Comm_rank ( MPI_COMM_WORLD, &id );
  
  
  //--------------------- Start of timer -------
  if ( id == 0 )
        wtime = MPI_Wtime( );
        
  // ---- handling larger cases ----------
  if( sqrt(limit) < (int)ceil( limit / p * 1.0 ) )
  {

  if ( id == 0 )
  { timestamp ( );
    printf ( "\n" );
    printf ( "PRIME_MPI\n" );
    printf ( "  C/MPI version\n" );
    printf ( "\n" );
    printf ( "  An MPI example program to count the number of primes.\n" );
    printf ( "  The number of processes is %d", p );
    printf ( "\n" );
    printf ( "         N        Pi          Time\n" );
    printf ( "\n" );
  }

  // ------------ create arrays ----------
  if (id != p-1)
    n_hi = (int)(limit/p) *(id+1);
  else 
    n_hi = limit;
  if(id == 0)
        n_lo = 2;  
  else
        n_lo = (int)(limit/p) *id;
        
  char *arr = (char*) calloc((n_hi - n_lo) , sizeof(char));
  ptr = n_lo;
  //find next prime

  while (ptr < limit)
  { //get next prime  
    if(id == 0)
    {   while(arr[ptr-n_lo] == 0)
          {  ptr+=1;    
            if(ptr == n_hi)
              {  ptr = 0;
                 break;
              }
           }                  
    }
    
    prime  = ptr;
  
  //broadcast prime
  ierr = MPI_Bcast( &prime, 1, MPI_CHAR, 0, MPI_COMM_WORLD );
  
  if(prime == 0)
    break;
  
  start = (int) ceil(n_lo/prime*1.0) *  prime ;
  
  //eliminate multiples of prime in n_lo to n_hi
  for(i = start - n_lo ;i<n_hi - n_lo ; i += prime )
    arr[i] = 1;
  
  }
  
  
  // count primes
  for(i = 0;i<n_hi-n_lo;i+=1)
    count+=arr[i];
  ierr = MPI_Reduce ( &count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD );
  
  if ( id == 0 )
    { wtime = MPI_Wtime( ) - wtime;
      printf ("  %8d  %8d  %14f\n", limit, total_count, wtime );
    }
  
  
 }
 else 
 {  // insert code for sqrt(limit) > limit/p
  
  }
  
  /*   Terminate MPI. */
  ierr = MPI_Finalize ( );
/*   Terminate. Code for performance measurement */
  if ( id == 0 ) 
  { printf ( "\n");         
    printf ( "PRIME_MPI - Master process:\n");         
    printf ( "  Normal end of execution.\n"); 
    printf ( "\n" );
    timestamp ( );        
  }
  return 0;
}



