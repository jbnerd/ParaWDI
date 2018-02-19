#include "bfilter.h"
#include "_TRIE.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned long* traverse( TrieNode *cnode , unsigned long* vec, char *key);
