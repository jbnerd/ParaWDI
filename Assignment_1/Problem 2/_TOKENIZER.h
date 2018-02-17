#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#ifndef _TOKEN
#define _TOKEN

int offset = 0;

bool eof = false;

typedef char* buffer;

char* getWord(FILE* fp, buffer b, int k);

char** list_dir(char* folder_name, int* file_count);

#endif