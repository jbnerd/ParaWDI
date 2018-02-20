#include "_TOKENIZER.h"
#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mpi.h>

void indexing();

void decompose_domain(int domain_size, int world_rank, int world_size, int* subdomain_start, int* subdomain_end) {
    /*******************************************************************
    * DESCRIPTION : Assign the subdomain to given process
    * NOTES:     
    *           [1] Argument domain_size: Size of the over all domain (In this case value of filecount)
    *
    */

    *subdomain_start = (int) (domain_size / world_size )  * world_rank;    
    
    if ( world_rank != world_size - 1)
        *subdomain_end = (int) (domain_size / world_size )  * (world_rank+1);
    else 
        *subdomain_end = domain_size;
    
}


int main(){

    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    TrieNode** clus_root;
    char* folder_name = "/home/shyamal/ST";
        
    indexing(clus_root, folder_name, world_rank, world_size);

    MPI_Finalize();
    return 0;
}


void indexing(TrieNode** clus_root, char* folder_name, int world_rank, int world_size ){

    int file_count;
    char** old_file_list = list_dir(folder_name, &file_count);
    char** file_list = append_paths(folder_name, old_file_list, file_count);

    int subdomain_start, subdomain_end;
    decompose_domain(file_count, world_rank, world_size, &subdomain_start, &subdomain_end);

    int i;
    buffer b = (buffer) malloc(4096 * sizeof(char));
    buffer word_buffer;
    
    int k = 4096;
    memset(b, 0, k);

    *clus_root = get_clus_Node();

    // printf("%d\n", subdomain_start);
    for(i = subdomain_start; i < subdomain_end ; i ++){
        // printf("%d : %s\n", world_rank ,file_list[i]);
        FILE* fp = fopen(file_list[i], "r");
        if(!fp){
            perror("");
            exit(1);
        }

        TrieNode* doc_root = get_doc_Node();
        offset = 0;
        memset(b, 0, k);
        // memset(word_buffer, 0, 25);
        eof = false;

        while(1){
            word_buffer = getWord(fp, b, k);
            word_buffer = convert_to_lower(word_buffer);
            doc_root = doc_insert(doc_root, word_buffer);
            free(word_buffer);
            if( offset >= strlen(b) && eof ) break;                       
        }
        
        fclose(fp);
        *clus_root = insert_doc_in_clus(*clus_root, doc_root, old_file_list[i]);
        doc_free(doc_root);
    
    }

    char* lol = "power";
    // clus_search(*clus_root, lol);

    int size1, size2, iter1, iter2;
    char *str1, *str2;
    if(world_rank != 0){
        size1 = 4096 ; size2 = 4096 ; iter1 = 0 ; iter2 = 0;
        str1 = (char*) malloc(size1 * sizeof(char));
        str2 = (char*) malloc(size2 * sizeof(char));
        memset(str1, '\0', size1); memset(str2, '\0', size2);
        serialize_trie(*clus_root, &str1, &str2, -1, &iter1, &iter2, &size1, &size2);
        // printf("%s\n", str1);
        // printf("%s\n", str2);
    }else{
        size1 = 0; size2 = 0; iter1 = 0; iter2 = 0;
        str1 = "\0"; str2 = NULL;
    }
    
    int *rcounts1, *rcounts2, *dsply1, *dsply2;
    if (world_rank == 0){
        rcounts1 = (int*) malloc(world_size * sizeof(int));
        rcounts2 = (int*) malloc(world_size * sizeof(int));
        dsply1 = (int*) malloc(world_size * sizeof(int));
        dsply2 = (int*) malloc(world_size * sizeof(int));
    }

    MPI_Gather(&size1, 1, MPI_INT, rcounts1, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gather(&size2, 1, MPI_INT, rcounts2, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int tcount1 = 0, tcount2 = 0;

    if(world_rank == 0){
        for(i=0; i < world_size; i++){
            dsply1[i] = tcount1;            
            tcount1 += rcounts1[i];
            dsply2[i] = tcount2;            
            tcount2 += rcounts2[i];
        }
    }

    char *whole_trie, *whole_list;
    if(world_rank == 0){
        whole_trie = (char*) malloc(tcount1 * sizeof(char));
        whole_list = (char*) malloc(tcount2 * sizeof(char));
    }

    MPI_Gatherv(str1, size1, MPI_CHAR, whole_trie, rcounts1, dsply1, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Gatherv(str2, size2, MPI_CHAR, whole_list, rcounts2, dsply2, MPI_CHAR, 0, MPI_COMM_WORLD);



    if(world_rank == 0){
    //     printf("*************\n");
    // printf("%s\n %s\n", whole_trie, &whole_trie[dsply1[2]]);
    // printf("%s\n %s\n", whole_list, &whole_list[dsply2[2]]);


        char** all_str1 = (char**) malloc((world_size -1) * sizeof(char*));
        char** all_str2 = (char**) malloc((world_size -1) * sizeof(char*));
        for (i = 0; i < world_size -1; i ++){
            all_str1[i] = (char*) malloc(rcounts1[i+1] * sizeof(char));
            memset(all_str1[i], '\0', rcounts1[i+1]);
            strcpy(all_str1[i], &whole_trie[dsply1[i+1]]);
            all_str2[i] = (char*) malloc(rcounts2[i+1] * sizeof(char));
            memset(all_str2[i], '\0', rcounts2[i+1]);
            strcpy(all_str2[i], &whole_list[dsply2[i+1]]);
        }


        TrieNode** deser_tries = (TrieNode**) malloc(world_size * sizeof(TrieNode*));
        int iter1, iter2;

        for(i = 1; i < world_size; i++){
            iter1 = 0; iter2 = 0;
            deser_tries[i] = deserialize_trie(deser_tries[i], &(all_str1[i-1]), &(all_str2[i-1]), &iter1, &iter2);
        }

        for(i = 1; i < world_size; i++){
            *clus_root = cluster_merge(*clus_root, deser_tries[i]);
        }
        clus_search(*clus_root, lol);
    }
}
