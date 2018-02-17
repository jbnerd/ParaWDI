#include "_TOKENIZER.h"
#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(){

    int file_count;
    char** file_list = list_dir(".", &file_count);

    int i;
    buffer b = (buffer) malloc(4096 * sizeof(char));
    int k = 4096;
    memset(b, 0, k);
    TrieNode* clus_root = get_clus_Node();

    for(i = 0; i < file_count; i ++){
        FILE* fp = fopen(file_list[i], "r");
        if(!fp){
            perror("");
            exit(1);
        }

        TrieNode* doc_root = get_doc_Node();
        buffer word_buffer = (buffer) malloc(25*sizeof(char));

        offset = 0;
        memset(b, 0, k);
        eof = false;

        while(1){
            word_buffer = getWord(fp, b, k);
            word_buffer = convert_to_lower(word_buffer);
            doc_root = doc_insert(doc_root, word_buffer);
            if( offset >= strlen(b) && eof ) break;                       
        }
        
        clus_root = insert_doc_in_clus(clus_root, doc_root, file_list[i]);
        free(doc_root);
        fclose(fp);
    }
    
    char* lol = "lol";
    clus_search(clus_root, lol);
        
    return 0;
}


