#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "_LINKED_LIST.h"

#ifndef _TRIE
#define _TRIE

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

struct ArrNode{
    unsigned int frequency;
    List* list;
};

typedef struct ArrNode ArrNode;

#define ALPHABET_SIZE 26

struct TrieNode{
    struct TrieNode** children; //[ALPHABET_SIZE];
    bool end;
    unsigned int frequency;
    List* list;
};

typedef struct TrieNode TrieNode;

TrieNode* get_clus_Node(void);

TrieNode* get_doc_Node(void);

TrieNode* doc_insert(TrieNode *root, char *key);

TrieNode* clus_insert(TrieNode *root, char *key, unsigned int frequency, char* doc_name);

bool doc_search(struct TrieNode *root, const char *key);

List* clus_search(struct TrieNode *root, const char *key);

TrieNode* cluster_merge(TrieNode *croot1,TrieNode *croot2);

char* convert_to_lower(char* str);

TrieNode * insert_doc_in_clus(TrieNode *croot, TrieNode *droot, char* doc_name );

void serialize_trie(TrieNode* root, char** fp1, char** fp2, int child_index, int* fp1_count, int* fp2_count, int* fp1_size, int* fp2_size);

TrieNode* deserialize_trie(TrieNode* root, char** fp1, char** fp2, int* fp1_count, int* fp2_count);

char index_to_char(int index);

int doc_free(TrieNode* dnode);

#endif
