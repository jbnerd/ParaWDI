# Run on Machine with MPICH2 installed.

## instructions to compile and run program
mpicc -g stage2.c -o prime -lm
mpirun -np 3 prime 100

# prime.c is a simple approch to find primes

# stage2.c is  a parallel version of the Sieve of Eratosthenes algorithm
- Still suffering from segfault issues. 
