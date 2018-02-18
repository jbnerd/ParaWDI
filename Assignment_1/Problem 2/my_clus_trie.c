#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include <string.h>

#define _CLUS_TRIE

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
 
TrieNode* get_clus_Node(void){
    TrieNode *temp = (TrieNode*) malloc(sizeof(TrieNode));
    temp -> children = (TrieNode**) malloc(ALPHABET_SIZE * sizeof(TrieNode*));

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
 
TrieNode* clus_insert(TrieNode *root, char *key, int frequency, char* doc_name){
    int depth, len = strlen(key), index;
    //root -> frequency += frequency;
 
    TrieNode *iter = root;
 
    for (depth = 0; depth < len; depth++){
        index = CHAR_TO_INDEX(key[depth]);
        if (!iter -> children[index]){
            iter -> children[index] = get_clus_Node();
        } 
        iter = iter -> children[index];
    }
    // iter -> frequency += frequency;
    iter -> end = true;

    Element* temp = (Element*) malloc(sizeof(Element));
    temp -> frequency = frequency;
    temp -> doc_name = (char*) malloc((strlen(doc_name) + 1) * sizeof(char));
    strcpy(temp -> doc_name, doc_name);
    iter -> list = insert_in_order(iter -> list, temp);
    return root;
}
 
bool clus_search(struct TrieNode *root, const char *key){
    int depth, len = strlen(key), index;
    TrieNode* iter = root;
 
    for (depth = 0; depth < len; depth++){
        index = CHAR_TO_INDEX(key[depth]);
        if (!iter -> children[index]){
            return false;
        }
        iter = iter -> children[index];
    }
    print_list(iter -> list);
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

char index_to_char(int index){
    return index + 'a';
}

/*
int main(){
    // Input keys (use only 'a' through 'z' and lower case)
    // char keys[][8] = {"the", "a", "there", "answer", "any", "by", "bye", "their"};
    char output[][32] = {"Not present in trie", "Present in trie"};
    char keys[][4] = {"a", "any", "by", "bye"};

//     TrieNode *root = get_clus_Node();
//     int i;
//     for (i = 0; i < ARRAY_SIZE(keys); i++){
//         clus_insert(root, keys[i], i, "lol");
//     }

    FILE* fptr1 = fopen("serial_trie.txt", "w+");
    FILE* fptr2 = fopen("serial_lists.txt", "w+");
    serialize_trie(root, &fptr1, &fptr2, -1);
    fclose(fptr1);
    fclose(fptr2);

    fptr1 = fopen("serial_trie.txt", "r");
    fptr2 = fopen("serial_lists.txt", "r");
    // FILE* fptr_temp = fopen("test.txt", "w+");
    TrieNode* de_root;
    de_root = deserialize_trie(de_root, &fptr1, &fptr2);
    // serialize_trie(de_root, fptr_temp, -1);
    fclose(fptr1);
    fclose(fptr2);
    // fclose(fptr_temp);

    printf("%d\n\n", clus_search(de_root, "answer"));
    printf("%d\n", clus_search(de_root, "a"));
    printf("%d\n", clus_search(de_root, "any"));
    printf("%d\n", clus_search(de_root, "by"));
    printf("%d\n", clus_search(de_root, "bye"));

    // fptr2 = fopen("serial_lists.txt", "r");
    // List* list = deserialize_list(fptr2);
    // print_list(list);
    // List* list1 = deserialize_list(fptr2);
    // print_list(list1);
    // List* list2 = deserialize_list(fptr2);
    // print_list(list2);
    // List* list3 = deserialize_list(fptr2);
    // print_list(list3);
    // List* list4 = deserialize_list(fptr2);
    // print_list(list4);
    // fclose(fptr2);
    // Search for different keys
    // printf("%s --- %s\n", "the", output[search(root, "the")] );
    // printf("%s --- %s\n", "these", output[search(root, "these")] );
    // printf("%s --- %s\n", "their", output[search(root, "their")] );
    // printf("%s --- %s\n", "thaw", output[search(root, "thaw")] );
//     // printf("%s --- %s\n", "these", output[search(root, "a")] );
//     // printf("%s --- %s\n", "these", output[search(root, "there")] );
 
    return 0;
}
*/