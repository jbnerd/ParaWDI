#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include "_TOKENIZER.h"

int offset = 0;
bool eof = false;

FILE *getStream(FILE *fp, buffer b, int k)
{	int count = -1;
	if (!eof)
        count = fread(b, 1, k, fp);
    //hack
    if(count < k) eof = true ;
    b[count] = '\0';
    return fp;
}

char* getWord(FILE* fp, buffer b, int k){
    buffer temp = (buffer) malloc(k * sizeof(char));
    int i = 0;
    memset(temp, 0, k);
    while(1){
        if (offset == k || strlen(b) == 0 || b[offset] == '\0'){
			if( eof ){
				return temp;
			}
			memset(b, 0, sizeof(b));
            fp = getStream(fp, b, k);
            offset = 0;
		}
        if(('a' <= b[offset] && b[offset] <= 'z') || ('A' <= b[offset] && b[offset] <= 'Z')){
            temp[i++] = b[offset++];
        }else{
            offset++;
            if(temp[0] == '\0') continue;
            else return temp;
        }
    }
}

char** list_dir(char* folder_name, int* file_count){
    DIR *dir;
    struct dirent *ent;
    *file_count = 0;
    if ((dir = opendir (folder_name)) != NULL) {
        /* Count total files within directory */
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) { /* If the entry is a regular file */
                (*file_count)++;
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return NULL;
    }

    char** file_list = (char**) malloc((*file_count) * sizeof(char*));
    int i = 0;
    
    if ((dir = opendir (folder_name)) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == DT_REG){
                file_list[i] =  (char*)malloc(strlen(ent->d_name)*sizeof(char));
                strcpy(file_list[i], ent->d_name);
                // printf ("%s\n", file_list[i]);
                i++;
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return NULL;
    }
    return file_list;
}


// int main(){
//     buffer b = (buffer) malloc(4096 * sizeof(char));
//     int k = 4096;
//     memset(b, 0, k);
//     int i = 0;
//     char* temp;
//     FILE* fp = fopen("test", "r");

//     // while(1){
//     //     printf("%s\n", getWord(fp, b, k));
//     //     if( offset >= strlen(b) && eof ) break;        
//     // }
    
//     int file_count;
//     char** filelist = list_dir(".", &file_count);
//     // printf("%d", file_count);
//     for(i=0; i < file_count; i++){
//         printf("%s\n", filelist[i]);
//     }

// }