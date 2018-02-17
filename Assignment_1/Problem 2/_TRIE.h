#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "_LINKED_LIST.h"

#ifndef _TRIE
#define _TRIE

struct ArrNode{
    unsigned int frequency;
    List* list;
};

typedef struct ArrNode ArrNode;

#define ALPHABET_SIZE 26

struct TrieNode{
    struct TrieNode *children[ALPHABET_SIZE];
    bool end;
    unsigned int frequency;
    List* list;
};

typedef struct TrieNode TrieNode;

TrieNode* getNode(void);

TrieNode* insert_clus(TrieNode *root, char *key, int frequency, char* doc_name);

TrieNode* insert_doc(TrieNode *root, char *key);

bool search(struct TrieNode *root, const char *key);

#endif
