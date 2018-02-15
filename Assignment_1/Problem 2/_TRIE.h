#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "_LINKED_LIST.h"

#ifndef _TRIE
#define _TRIE

#define ALPHABET_SIZE 26

struct TrieNode{
    struct TrieNode *children[ALPHABET_SIZE];
    bool end;
    unsigned int frequency;
    List* list;
};
typedef struct TrieNode TrieNode;

TrieNode* insert(TrieNode *root, const char *key);

bool search(struct TrieNode *root, const char *key);

#endif