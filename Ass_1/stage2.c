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
  //setting limit
  if (argc > 0)
    limit = (int)atoi(argv[1]);
  else
    limit = UPPER_BOUND;     /*setting upper bound */  
  int lsqrt = (int)ceil(sqrt(limit));
  
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
  if( lsqrt < (int)ceil( limit / p * 1.0 ) )
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
  }
  int root = 0;
  // ------------ create arrays ----------
  if (id != p-1)
    n_hi = (int)(limit/p) *(id+1);
  else 
    n_hi = limit;
  if(id == root)
        n_lo = 2;  
  else
        n_lo = (int)(limit/p) *id;    
  char *arr = (char*) calloc((n_hi - n_lo) , sizeof(char));
  

  printf ( " \n Array Created %8d , %8d limits.\n", n_hi , n_lo );
  //find next prime
  ptr = n_lo;
  while(1)
  {     //get next prime  
        if(id == root)
        {   while(arr[ptr-n_lo] == 1)
              {  ptr+=1;    
                 if(ptr > lsqrt)  // can comment out maybe
                  {  ptr = limit+1;
                     break;
                  }
               }    
            prime  = ptr;  
            ptr += 1;           
        }
    
    
    //broadcast prime
    ierr = MPI_Bcast( &prime, 1, MPI_CHAR, root, MPI_COMM_WORLD );
    printf ( "  prime is  %d, %d \n" , prime, id);
    if(prime >= n_hi)
        break;
    //eliminate multiples of prime in n_lo to n_hi
    start = (int) ceil(n_lo/prime*1.0) *  prime ;
    
    if(id == root)
        start += prime;
    for(i = start - n_lo ;i<n_hi - n_lo ; i += prime )
        arr[i] = 1;  
  }
  
  printf ( " marked composite.\n" );
  // count primes
  if(id = p-1)
  {  for(i = 0;i<n_hi-n_lo;i+=1)
    count+= 1 - arr[i];
  }
  else
  { for(i = 0;i<limit-n_lo;i+=1)
    count+= 1 - arr[i];
  
  }
  free(arr);
  ierr = MPI_Reduce ( &count, &total_count, 1, MPI_INT, MPI_SUM, root , MPI_COMM_WORLD );
  if ( id == root )
    { wtime = MPI_Wtime( ) - wtime;
      printf ( "         N        Pi          Time\n" );
      printf ( "\n" );
      printf ("  %8d  %8d  %14f\n", limit, total_count, wtime );
    } 
 }
 else 
 {  
    if ( id == 0 )
    {   timestamp ( );
        printf ( "\n" );
        printf ( "PRIME_MPI\n" );
        printf ( "  C/MPI version\n" );
        printf ( "\n" );
        printf ( "  An MPI example program to count the number of primes.\n" );
        printf ( "  The number of processes is %d but using only root process due to small input.", p );
        printf ( "\n" );
        char *arr = (char*) calloc((limit - 1) , sizeof(char));
        ptr = 2;
        while(ptr<=lsqrt)
        {   
            if(arr[ptr] == 0)
            {   for( i = 2*ptr - 2; i <= limit - 2; i += prime )
                        arr[i] = 1;                     
            }
        }
        for(i = 0; i < limit - 2; i++)
        {   count+=arr[i];
        }

        wtime = MPI_Wtime( ) - wtime;
        printf ( "         N        Pi          Time\n" );
        printf ( "\n" );
        printf ("  %8d  %8d  %14f\n", limit, total_count, wtime );
    }
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



