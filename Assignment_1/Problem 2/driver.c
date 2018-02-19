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
    char* folder_name = "/home/shyamal/corpora/reuters/training";
        
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

        //printf("%s\n", file_list[i]);
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

        // char c[2];
        // c[0] = i;
        // c[1] = '\0'
        *clus_root = insert_doc_in_clus(*clus_root, doc_root, old_file_list[i]);
        // free(doc_root);

        // int k = doc_free(doc_root);
        // printf(" %d  lol\n",k);
        
    }
        char* lol = "power";
        clus_search(*clus_root, lol);
    
}
