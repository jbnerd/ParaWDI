#include "_TRIE.h"
#include "_LINKED_LIST.h"

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

TrieNode* getNode(void){
    TrieNode *temp = (TrieNode*) malloc(sizeof(TrieNode));
 
    if (temp){
        int i;
        temp -> end = false;
        temp -> frequency = 0;
        temp -> list = create_list();
        for (i = 0; i < ALPHABET_SIZE; i++){
            temp -> children[i] = NULL;
        }
    } 
    return temp;
}
 
TrieNode* insert(TrieNode *root, const char *key){
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
    printf("%d\n", iter -> frequency);
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

int main(){
    // Input keys (use only 'a' through 'z' and lower case)
    char keys[][8] = {"the", "the", "the", "a", "there", "answer", "any", "by", "bye", "their"};
    char output[][32] = {"Not present in trie", "Present in trie"};
 
    TrieNode *root = getNode();
    int i;
    for (i = 0; i < ARRAY_SIZE(keys); i++){
        insert(root, keys[i]);
    }
 
    // Search for different keys
    printf("%s --- %s\n", "the", output[search(root, "the")] );
    printf("%s --- %s\n", "these", output[search(root, "these")] );
    printf("%s --- %s\n", "their", output[search(root, "their")] );
    printf("%s --- %s\n", "thaw", output[search(root, "thaw")] );
 
    return 0;
}