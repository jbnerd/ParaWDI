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

TrieNode* clus_insert(TrieNode *root, char *key, int frequency, char* doc_name);

bool doc_search(struct TrieNode *root, const char *key);

bool clus_search(struct TrieNode *root, const char *key);

void cluster_merge(TrieNode *croot1,TrieNode *croot2);

char* convert_to_lower(char* str);

TrieNode * insert_doc_in_clus(TrieNode *croot, TrieNode *droot, char* doc_name );

void serialize_trie(TrieNode* root, FILE** fp1, FILE** fp2, int child_index);

TrieNode* deserialize_trie(TrieNode* root, FILE** fp1, FILE** fp2);

char index_to_char(int index);

#endif
