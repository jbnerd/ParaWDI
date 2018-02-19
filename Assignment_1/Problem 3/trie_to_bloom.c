#include "_TRIE_BLOOM.h"

unsigned long* traverse(TrieNode *cnode, unsigned long* vec, char *key, depth)
{
    int i = 0, d = depth + 1;
    if( cnode -> end)
        vec = addB( key, vec);   
    for( i = 0; i < ALPHABET_SIZE; i++ )
    {   if( cnode -> children[i] ){
            key[depth] = i +'a';
            key[depth + 1] = '\0'; 
            cnode -> children[i] = traverse(cnode -> children[i], vec , key, d );
        }
    }
    return cnode;
}

unsigned long* clusToBloom(TrieNode *root){
    int depth = 0;
    char unsigned long* vec = getVector();
    char* key = (char*) malloc( 26 * sizeof(char));
    memset(key, 0, 25);
    return traverse(root, vec, key, depth);
}
