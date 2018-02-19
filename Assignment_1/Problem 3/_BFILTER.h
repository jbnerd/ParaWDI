#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef _BLOOM
#define _BLOOM

// only multiples of 64
#define FILTER_SIZE 4096
#define NUM_HASHES 8
#define SIZE 16
#define DOMAIN 262144


unsigned long* getVector();

unsigned long* addB(char *key, unsigned long* vector);

bool searchB(char *key, unsigned long* vector);

#endif