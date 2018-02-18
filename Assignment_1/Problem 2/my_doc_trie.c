#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include <string.h>

#define _DOC_TRIE

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

TrieNode* get_doc_Node(void){
    TrieNode *temp = (TrieNode*) malloc(sizeof(TrieNode));
    temp -> children = (TrieNode**) malloc(ALPHABET_SIZE * sizeof(TrieNode*));
 
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
 
TrieNode* doc_insert(TrieNode *root, char *key){
    int depth, len = strlen(key), index;
 
    TrieNode *iter = root;
 
    for (depth = 0; depth < len; depth++){
        index = CHAR_TO_INDEX(key[depth]);
        if (!iter -> children[index]){
            iter -> children[index] = get_doc_Node();
        } 
        iter = iter -> children[index];
    }
    iter -> frequency ++;
    iter -> end = true;

    return root;
}
 
bool doc_search(struct TrieNode *root, const char *key){
    int depth, len = strlen(key), index;
    TrieNode* iter = root;
 
    for (depth = 0; depth < len; depth++){
        index = CHAR_TO_INDEX(key[depth]);
        if (!iter -> children[index]){
            return false;
        }
        iter = iter -> children[index];
    }
    printf("%d\n", iter->frequency);
    return (iter != NULL && iter -> end);
}


// int main(){
//     // Input keys (use only 'a' through 'z' and lower case)
//     char keys[][8] = {"the", "a", "there", "answer", "any", "by", "bye", "their","the", "a", "there","the", "a", };
//     char output[][32] = {"Not present in trie", "Present in trie"};
 
//     TrieNode *root = get_doc_Node();
//     int i;
//     for (i = 0; i < ARRAY_SIZE(keys); i++){
//         doc_insert(root, keys[i]);
//     }
 
//     // Search for different keys
//     printf("%s --- %s\n", "the", output[doc_search(root, "the")] );
//     printf("%s --- %s\n", "these", output[doc_search(root, "these")] );
//     printf("%s --- %s\n", "their", output[doc_search(root, "their")] );
//     printf("%s --- %s\n", "thaw", output[doc_search(root, "thaw")] );

//     printf("%s --- %s\n", "a", output[doc_search(root, "a")] );
//     printf("%s --- %s\n", "there", output[doc_search(root, "there")] );
 
//     return 0;
// }
