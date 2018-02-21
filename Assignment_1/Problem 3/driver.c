#include "_TOKENIZER.h"
#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include "_BFILTER.h"
#include "_TRIE_BLOOM.h"
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
    // char* folder_name = "/home/shyamal/corpora/rt/training";
    char* folder_name = "/home/shyamal/para/corpora/reuters/test";
    char* query_folder = "/home/shyamal/para/query";

    indexing(clus_root, folder_name, query_folder,  world_rank, world_size);

    MPI_Finalize();
    return 0;
}


void indexing(TrieNode** clus_root, char* folder_name,  char* query_folder , int world_rank, int world_size){

    int file_count;
    char** old_file_list = list_dir(folder_name, &file_count);
    char** file_list = append_paths(folder_name, old_file_list, file_count);

    int query_count;
    char** query_list = list_dir(query_folder, &query_count);
    query_list = append_paths(query_folder, query_list, query_count);

    int subdomain_start, subdomain_end;
    decompose_domain(file_count, world_rank, world_size, &subdomain_start, &subdomain_end);

    int i;
    buffer b = (buffer) malloc(4096 * sizeof(char));
    buffer word_buffer;
    
    int k = 4096;
    memset(b, 0, k);

    *clus_root = get_clus_Node();

    unsigned long* bloom = getVector();

    FILE* fptr = fopen("stopwords", "r");
    int num_stop;
    char** list_of_words = read_arr(fptr, &num_stop);
    fclose(fptr);
    // printf("%d", num_stop);

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
            if(search(list_of_words, word_buffer, 0, num_stop - 1) == 1){
                free(word_buffer);   
                continue;  
            }
            doc_root = doc_insert(doc_root, word_buffer);
            bloom = addB(word_buffer, bloom);
            free(word_buffer);
            if( offset >= strlen(b) && eof ) break;                       
        }

        fclose(fp);
        *clus_root = insert_doc_in_clus(*clus_root, doc_root, old_file_list[i]);
        doc_free(doc_root);
    }

    unsigned long* all_blooms = (unsigned long*) malloc(FILTER_SIZE * world_size * sizeof(unsigned long));
    MPI_Allgather(bloom, FILTER_SIZE, MPI_UNSIGNED_LONG, all_blooms, FILTER_SIZE, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);

    int max_deser = 40 * (file_count - ((file_count / world_size )  * (world_size - 1))) + 60;

    int master = 0;
    int finish = 0;
   
    // corrusponding to master
    // char* fname = (char*) calloc(20, sizeof(char));
    // sprintf(fname, "./query%d.in", world_rank);
    // printf("%d :: %s\n", world_rank, query_list[world_rank]);
    
    FILE* fp = fopen(query_list[world_rank], "r");

    int who[world_size];

    i = 0;
    
    char* deser_data = (char*) calloc(max_deser, sizeof(char));

    List** all_lists = (List**) malloc(world_size * sizeof(List*));
    List* Final_list;

    int num_words;
    char** all_words = read_arr(fp, &num_words);
    fclose(fp);

    int finish_words = 0, remaining_cycle_work = 0;

    // corrusponding to slave
    char* query = (char*) malloc(140 * sizeof(char));
    int size2, iter2;
    char* str2;
    List* list;

    int pending_master[world_size];
    memset(pending_master, 1, world_size * sizeof(int));

    while(!finish){
        MPI_Barrier(MPI_COMM_WORLD);
        // MASTER NODE
        if(world_rank == master){
            remaining_cycle_work = 10;
            while( remaining_cycle_work-- ){
                
                word_buffer = convert_to_lower(all_words[finish_words++]);
                Final_list = create_list();
                memset(who, 0, world_size * sizeof(int));

                for(i = 0; i < world_size; i++ ){
                    if(i != world_rank){
                        who[i] = searchB(word_buffer, all_blooms + i * FILTER_SIZE);
                    }
                }

                for(i = 0; i < world_size; i++){
                    if(who[i]){
                        // printf("Sending: %s :: process - %d :: present - %d\n", word_buffer, world_rank, who[i]);
                        MPI_Send(word_buffer, strlen(word_buffer) + 1, MPI_CHAR, i, 1, MPI_COMM_WORLD);
                    }
                }


                
                if(searchB(word_buffer, all_blooms + world_rank * FILTER_SIZE)){
                    List* lst = clus_search(*clus_root, word_buffer);
                    if(lst){
                        Final_list = mergeLists(Final_list, lst);
                    }
                }

                for(i = 0; i < world_size; i++){
                    if(who[i]){
                        // MPI_Status* status;
                        // printf("Recieving :: %s :: process - %d :: present - %d\n", word_buffer, world_rank, who[i]);
                        MPI_Recv(deser_data, max_deser, MPI_CHAR, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        // printf("Recieved :: %s :: process - %d :: present - %d\n", deser_data, world_rank, who[i]);
                        // printf("%d ::  %s\n",i , deser_data);
                        iter2 = 0;
                        all_lists[i] = deserialize_list(&deser_data, &iter2);
                        memset(deser_data, 0,max_deser );
                    }
                }
                
                
                for(i=0; i< world_size; i++){
                    if(who[i]){
                        Final_list = mergeLists(Final_list, all_lists[i]);
                    }
                }
                printf("Process %d ::  :: %s\n",world_rank, word_buffer);
                print_list(Final_list);
                // free(Final_list);
                
                // ADD MASTER ARRAY .

                if(finish_words >= num_words){
                    strcpy(word_buffer, "$");
                    for(i = 0; i < world_size; i++ ){
                        if(i != world_rank){
                            // printf("Ending :: %s :: process - %d \n", word_buffer, world_rank);
                            MPI_Send(word_buffer, strlen(word_buffer) + 1, MPI_CHAR, i, 1, MPI_COMM_WORLD);
                            // printf("Ended :: %s :: process - %d \n", word_buffer, world_rank);
                        }
                    }

                    pending_master[master] = 0;
                    finish = 1;
                    i = 0;
                    master = (master + 1) % world_size;
                    while( i < world_size){
                        if(pending_master[(master + i) % world_size]){
                            master = master + i;
                            finish = 0;
                            break;
                        }
                        i++;
                    }

                    free(word_buffer);
                    break;
                }
                
                if( ! remaining_cycle_work ) {
                    strcpy(word_buffer, "!");
                    for(i = 0; i < world_size; i++ ){
                        if(i != world_rank){
                            // printf("Ending :: %s :: process - %d \n", word_buffer, world_rank);
                            MPI_Send(word_buffer, strlen(word_buffer) + 1, MPI_CHAR, i, 1, MPI_COMM_WORLD);
                            // printf("Ended :: %s :: process - %d \n", word_buffer, world_rank);
                        }
                    }
                    master = (master + 1) % world_size;
                    free(word_buffer);
                }
            }
        }
        // SLAVE NODES
        else{

            while(1){
                memset(query, 0, 140*sizeof(char));
                
                MPI_Recv(query, 140, MPI_CHAR, master, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
                
                // printf("Slave : Received :: %s :: process - %d \n", query, world_rank);
                
                if(strcmp(query, "!") == 0 ){
                    master = (master + 1) % world_size;
                    break;
                }
                else if(strcmp(query, "$") == 0 ){
                    pending_master[master] = 0;
                    finish = 1;
                    i = 0;
                    master = (master + 1) % world_size;
                    while( i < world_size){
                        if(pending_master[(master + i) % world_size]){
                            master = master + i;
                            finish = 0;
                            break;
                        }
                        i++;
                    }
                    break;
                }

                list = clus_search(*clus_root, query);
                if(list){
                    size2 = max_deser, iter2 = 0;
                    str2 = (char*) malloc(size2 * sizeof(char));
                    memset(str2, 0, size2);
                    serialize_list(list, &str2, &iter2, &size2);
                    // printf("Slave : Data: %s :: len(Data): %ld :: size2: %d \n", str2, strlen(str2), size2);
                }else{
                    str2 = "-1\n";
                    size2 = 4;
                }
                MPI_Send(str2, size2, MPI_CHAR, master, 2, MPI_COMM_WORLD);
                free(str2);
            }
        }
    }
}
