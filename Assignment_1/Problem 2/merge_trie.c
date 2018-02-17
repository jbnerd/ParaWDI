#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void plug(TrieNode *cnode, char* doc_name, int frequency)
{
    Element* temp = (Element*) malloc(sizeof(Element));
    temp -> doc_name = (char*) malloc((strlen(doc_name) + 1) * sizeof(char));
    temp -> frequency += frequency;
    strcpy(temp -> doc_name, doc_name);
    cnode -> list = insert_in_order(cnode -> list, temp);
    cnode -> end = true;
}


int dfs(TrieNode *cnode,TrieNode *dnode, char* doc_name )
{   int i = 0, j = 0;  
    if( dnode -> end )
    {
        plug( cnode , doc_name , dnode -> frequency );
        j+=1;
    }
    
    for( i = 0; i < ALPHABET_SIZE; i++ )
    {
        if( dnode -> children[i] )
        {
            if (!cnode -> children[i])
                cnode -> children[i] = getNode();
            j += dfs(cnode -> children[i],dnode -> children[i], doc_name );
        }     
    }
    return j;
}


void insert_doc_in_clus(TrieNode *croot,TrieNode *droot, char* doc_name )
{   
    croot -> frequency += dfs( croot, droot, doc_name );
}


void cluster_merge(TrieNode *croot1,TrieNode *croot2)
{
    int i = 0;  
    if( croot2 -> end )
    {
        croot-> list = mergeLists( croot1->list , croot2->list );
    }   
    for( i = 0; i < ALPHABET_SIZE; i++ )
    {
        if( croot2 -> children[i] )
        {
            if (!croot1 -> children[i])
                croot1 -> children[i] = getNode();
            cluster_merge( croot1 -> children[i],croot2 -> children[i] );
        }     
    }
}

int main()
{

}






