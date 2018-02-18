#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// only multiples of 64
#define FILTER_SIZE 16
#define NUM_HASHES 8
#define SIZE 16
#define DOMAIN 65536


unsigned long* getVector();
unsigned long* add(char *key, unsigned long* vector);
bool search(char *key, unsigned long* vector);
