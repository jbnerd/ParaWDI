#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void serialize_trie(TrieNode* root, FILE** fp1, FILE** fp2, int child_index){
    if(root == NULL){
    	return;
    }

    if(child_index == -1){
 		fprintf(*fp1, "%c ", '$');
 	}else{
 		if(root -> end){
    		fprintf(*fp1, "%c ", index_to_char(child_index));
    		fprintf(*fp1, "%c ", '(');
    		serialize_list(root -> list, fp2);
    	}else{
    		fprintf(*fp1, "%c ", index_to_char(child_index));
    	}
 	}

    int i;
 	for(i = 0; i < ALPHABET_SIZE; i++){
       	serialize_trie(root -> children[i], fp1, fp2, i);
    }
 
    fprintf(*fp1, "%c ", '|');
}


TrieNode* deserialize_trie(TrieNode* root, FILE** fp1, FILE** fp2){
    char val;
	int garbage;

    while(1){
	    garbage = fscanf(*fp1, "%c ", &val);
    	// printf("%c ", val);

    	if(feof(*fp1) || garbage == 0){
    		break;
    	}

	    if(garbage != 0){
	    	if(val == '|'){
				return root;
	    	}else if(val == '$'){
	    		root = get_clus_Node();
	    		root = deserialize_trie(root, fp1, fp2);
	    		break;
	    	}else if(val == '('){
		    	root -> end = 1;
		    	root -> list = deserialize_list(fp2);
	    		root = deserialize_trie(root, fp1, fp2);
	    		break;
	    	}else{
		    	int index = CHAR_TO_INDEX(val);
	    		root -> children[index] = get_clus_Node();
	    		root -> children[index] = deserialize_trie(root -> children[index], fp1, fp2);
	    	}
	    }
	}

	return root;
}

void serialize_list(List* list, FILE** fp){
	Node* temp = list -> head;
	while(temp != NULL){
		fprintf(*fp, "%d %s\t", temp -> ele -> frequency, temp -> ele -> doc_name);
		temp = temp -> next;
	}
	fprintf(*fp, "-1\n");
}

List* deserialize_list(FILE** fp){
	List* list = create_list();
	int garbage, frequency;
	char* temp_str = (char*) malloc(25 * sizeof(char));

	while(1){
		Element* data = (Element*) malloc(sizeof(Element));
		data -> doc_name = (char*) malloc(25 * sizeof(char));
		garbage = fscanf(*fp, "%d", &frequency);
		if(frequency == -1){
			break;
		}
		garbage = fscanf(*fp, "%s", temp_str);

		if(feof(*fp) || garbage == 0){
			break;
		}

		data -> frequency = frequency;
		strcpy(data -> doc_name, temp_str);

		list = add_to_end(list, data);
	}

	return list;
}