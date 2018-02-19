#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#ifndef _TOKEN
#define _TOKEN

extern int offset;

extern bool eof;

typedef char* buffer;

char* getWord(FILE* fp, buffer b, int k);

char** list_dir(char* folder_name, int* file_count);

char** append_paths(char* folder_name, char** file_list, int file_count);

#endif