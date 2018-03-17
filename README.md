NOTE: @[jbnerd](https://github.com/jbnerd) will take care of this README !

# Dependencies
* MPICH2

## Instructions
```shell
mpicc -g stage2.c -o prime -lm
mpirun -np 3 prime 100
```