#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void serialize_trie(TrieNode* root, FILE *fp, int child_index){
    if(root == NULL){
    	return;
    }

    if(child_index == -1){
 		fprintf(fp, "%c ", '$');
 	}else{
 		if(root -> end){
    		fprintf(fp, "%c ", index_to_char(child_index));
    		fprintf(fp, "%c ", '(');
    	}else{
    		fprintf(fp, "%c ", index_to_char(child_index));
    	}
 	}

    int i;
 	for(i = 0; i < ALPHABET_SIZE; i++){
       	serialize_trie(root -> children[i], fp, i);
    }
 
    fprintf(fp, "%c ", '|');
}


TrieNode* deSerialize(TrieNode* root, FILE *fp){
    char val;
	int garbage;

    while(1){
	    garbage = fscanf(fp, "%c ", &val);
    	printf("%c ", val);

    	if(feof(fp)){
    		break;
    	}

	    if(garbage != 0){
	    	if(val == '|'){
				return root;
	    	}else if(val == '$'){
	    		root = get_clus_Node();
	    		root = deSerialize(root, fp);
	    		break;
	    	}else if(val == '('){
		    	root -> end = 1;
	    		root = deSerialize(root, fp);
	    		break;
	    	}else{
		    	int index = CHAR_TO_INDEX(val);
	    		root -> children[index] = get_clus_Node();
	    		root -> children[index] = deSerialize(root -> children[index], fp);
	    	}
	    }
	}

	return root;
}