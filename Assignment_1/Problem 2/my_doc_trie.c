#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include <string.h>

#define _DOC_TRIE

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

TrieNode* getNode(void){
    TrieNode *temp = (TrieNode*) malloc(sizeof(TrieNode));
 
    if (temp){
        int i;
        temp -> end = false;
        temp -> frequency = 0;
        for (i = 0; i < ALPHABET_SIZE; i++){
            temp -> children[i] = NULL;
        }
    } 
    return temp;
}
 
TrieNode* insert(TrieNode *root, char *key){
    int depth, len = strlen(key), index;
 
    TrieNode *iter = root;
 
    for (depth = 0; depth < len; depth++){
        index = CHAR_TO_INDEX(key[depth]);
        if (!iter -> children[index]){
            iter -> children[index] = getNode();
        } 
        iter = iter -> children[index];
    }
    iter -> frequency ++;
    iter -> end = true;

    return root;
}
 
bool search(struct TrieNode *root, const char *key){
    int depth, len = strlen(key), index;
    TrieNode* iter = root;
 
    for (depth = 0; depth < len; depth++){
        index = CHAR_TO_INDEX(key[depth]);
        if (!iter -> children[index]){
            return false;
        }
        iter = iter -> children[index];
    }
    return (iter != NULL && iter -> end);
}

char* convert_to_lower(char* str){
    int i;
    for(i = 0; str[i] != '\0'; i++){
        if(str[i] >= 'A' && str[i] <= 'Z'){
            str[i] += 'a' - 'A';
        }
    }
    return str;
}