#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void plug(TrieNode *cnode, char* doc_name, unsigned int frequency)
{
    Element* temp = (Element*) malloc(sizeof(Element));
    temp -> doc_name = (char*) malloc((strlen(doc_name) + 1) * sizeof(char));
    temp -> frequency = frequency;
    strcpy(temp -> doc_name, doc_name);
    cnode -> list = insert_in_order(cnode -> list, temp);
    cnode -> end = true;
}


TrieNode* dfs(TrieNode *cnode,TrieNode *dnode, char* doc_name )
{   int i = 0;  
    if( dnode -> end)
    {
        plug( cnode , doc_name , dnode -> frequency );
    }
    
    for( i = 0; i < ALPHABET_SIZE; i++ )
    {
        if( dnode -> children[i] )
        {
            if (!cnode -> children[i])
                cnode -> children[i] = get_clus_Node();
            cnode -> children[i] = dfs(cnode -> children[i],dnode -> children[i], doc_name );
        }     
    }
    return cnode;
}

TrieNode* insert_doc_in_clus(TrieNode *croot,TrieNode *droot, char* doc_name )
{   
    return dfs( croot, droot, doc_name );
}


TrieNode* cluster_merge(TrieNode *croot1,TrieNode *croot2)
{
    int i = 0;  
    if( croot2 -> end )
    {
        croot1-> list = mergeLists( croot1->list , croot2->list );
    }   
    for( i = 0; i < ALPHABET_SIZE; i++ )
    {
        if( croot2 -> children[i] )
        {
            if (!croot1 -> children[i])
                croot1 -> children[i] = get_clus_Node();
            croot1 -> children[i] = cluster_merge( croot1 -> children[i],croot2 -> children[i] );
        }     
    }
    return croot1;
}


int doc_free(TrieNode* dnode){
    int i,k=1;
    for(i=0; i< ALPHABET_SIZE; i++){
        if(dnode -> children[i])
            k+=doc_free( dnode -> children[i]);
    }
    free(dnode);
    return k;
}