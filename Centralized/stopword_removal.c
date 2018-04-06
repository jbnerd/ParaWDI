#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char** read_arr(FILE* fptr, int* size){

    *size = 32;
    char** list_of_words = (char**) malloc((*size) * sizeof(char*));
    int i = 0, garbage;
    for(i = 0; i < 571; i++){
        list_of_words[i] = (char*) malloc(25* sizeof(char));
        garbage = fscanf(fptr, "%s", list_of_words[i]);
        //printf("%s\n", list_of_words[i]);
        
        if(i >= (*size) - 2){
            list_of_words = (char**) realloc(list_of_words, (*size) * 2 * sizeof(char**));
            (*size) = (*size) * 2;
        }
    }


    *size = 571;

    return list_of_words;
}

bool search(char** arr, char* key, int lo, int hi){

    int what;
    while(lo <= hi){
        int mid = (lo + hi)/2;
        if((what = strcmp(arr[mid], key)) == 0){
            return true;
        }else if(what > 0){
            hi = mid - 1;
        }else{
            lo = mid + 1;
        }
    }
    return false;
}

int main(){

    FILE* fptr = fopen("stopwords", "r");
    if(fptr != NULL){
        printf("File opened correctly\n");
    }

    int size;
    char** list_of_words = read_arr(fptr, &size);
    
    fclose(fptr);

    printf("%d\n", search(list_of_words, "zerot", 0, 570));

    return 0;
}
