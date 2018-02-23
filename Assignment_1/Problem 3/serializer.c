#include "_TRIE.h"
#include "_LINKED_LIST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void MAY_REALLOC(char** fp, int* fp_count, int* fp_size){
    if((*fp_count) >= (*fp_size) - 50){
        int size = (*fp_size);
        *fp = (char*) realloc(*fp, 2 * size * sizeof(char));
        memset((*fp) + (*fp_size), '\0', *fp_size);
        (*fp_size) = 2 * size;
    }
}

void serialize_trie(TrieNode* root, char** fp1, char** fp2, int child_index, int* fp1_count, int* fp2_count, int* fp1_size, int* fp2_size){
    if(root == NULL){
    	return;
    }

    if(child_index == -1){
        *(*fp1 + *fp1_count) = '$';
        (*fp1_count) ++;
        MAY_REALLOC(fp1, fp1_count, fp1_size);
 		// fprintf(*fp1, "%c ", '$');
 	}else{
 		if(root -> end){
    		// fprintf(*fp1, "%c ", index_to_char(child_index));
    		// fprintf(*fp1, "%c ", '(');
            *(*fp1 + *fp1_count) = index_to_char(child_index);
            (*fp1_count) ++;
            MAY_REALLOC(fp1, fp1_count, fp1_size);
            *(*fp1 + *fp1_count) = '(';
            (*fp1_count) ++;
            MAY_REALLOC(fp1, fp1_count, fp1_size);
    		serialize_list(root -> list, fp2, fp2_count, fp2_size);
    	}else{
    		// fprintf(*fp1, "%c ", index_to_char(child_index));
            *(*fp1 + *fp1_count) = index_to_char(child_index);
            (*fp1_count) ++;
            MAY_REALLOC(fp1, fp1_count, fp1_size);
    	}
 	}

    int i;
 	for(i = 0; i < ALPHABET_SIZE; i++){
       	serialize_trie(root -> children[i], fp1, fp2, i, fp1_count, fp2_count, fp1_size, fp2_size);
    }
 
    // fprintf(*fp1, "%c ", '|');
    *(*fp1 + *fp1_count) = '|';
    (*fp1_count) ++;
    MAY_REALLOC(fp1, fp1_count, fp1_size);
}


TrieNode* deserialize_trie(TrieNode* root, char** fp1, char** fp2, int* fp1_count, int* fp2_count){
    char val;
	int garbage;

    while(1){
	    // garbage = fscanf(*fp1, "%c ", &val);
        val = *(*fp1 + *fp1_count);
        (*fp1_count) ++;
    	// printf("%c ", val);

    	// if(feof(*fp1) || garbage == 0){
    	// 	break;
    	// }

	    // if(garbage != 0){
	    if(val == '|'){
			return root;
	    }else if(val == '$'){
	    	root = get_clus_Node();
	    	root = deserialize_trie(root, fp1, fp2, fp1_count, fp2_count);
	    	break;
	    }else if(val == '('){
		   	root -> end = 1;
		   	root -> list = deserialize_list(fp2, fp2_count);
	    	root = deserialize_trie(root, fp1, fp2, fp1_count, fp2_count);
	    	break;
	    }else{
		   	int index = CHAR_TO_INDEX(val);
	    	root -> children[index] = get_clus_Node();
	    	root -> children[index] = deserialize_trie(root -> children[index], fp1, fp2, fp1_count, fp2_count);
	    }
	    // }
	}

	return root;
}

char* itoa(int data){
	char* ret = (char*) malloc(10*sizeof(char));
	memset(ret, '\0', 10 * sizeof(char));
	sprintf(ret, "%d", data);
	return ret;
}

void serialize_list(List* list, char** fp, int* fp_count, int* fp_size){
	Node* temp = list -> head;
	char* temp_fp = *fp;
    // printf("%d\n", list  -> size);
    // char* store = (char*) malloc(100 * sizeof(char));
	while(temp != NULL){
		// sprintf(store, "%d\t%s\t", temp -> ele -> frequency, temp -> ele -> doc_name);
  //       strcat(*fp, store);

		char* buf = itoa(temp -> ele -> frequency);
		int len = strlen(buf);
		memcpy(temp_fp + *fp_count, buf, len);
		*fp_count += len;
		memcpy(temp_fp + *fp_count, "\t", sizeof(char));
		*fp_count += 1;
		len = strlen(temp -> ele -> doc_name);
		memcpy(temp_fp + *fp_count, temp -> ele -> doc_name, len);
		*fp_count += len;
		memcpy(temp_fp + *fp_count, "\t", sizeof(char));
		*fp_count += 1;

        // while(*(*fp + *fp_count) != '\0'){
        //     (*fp_count) ++;
        // }
		if((*fp_count) >= (*fp_size) - 50){
			MAY_REALLOC(fp, fp_count, fp_size);
			temp_fp = *fp;
		}
		temp = temp -> next;
	}
	// sprintf(store, "-1\n");
    // strcat(*fp, store);
    memcpy(temp_fp + *fp_count, "-1\n", strlen("-1\n"));
    *fp_count += strlen("-1\n");
}

List* deserialize_list(char** fp, int* fp_count){
	List* list = create_list();
	int garbage, len;
    unsigned int frequency;
	char* temp_str = (char*) malloc(100 * sizeof(char));

	while(1){
		Element* data = (Element*) malloc(sizeof(Element));
		
		garbage = sscanf((*fp + *fp_count), "%d", &frequency);
		char* freq_str = itoa(frequency);
		len = strlen(freq_str);
		(*fp_count) += len + 1;
        // while(*(*fp + *fp_count) != '\t' && *(*fp + *fp_count) != '\n'){
        //     (*fp_count) ++;
        // }
        // (*fp_count) ++;
		if(frequency == -1){
			break;
		}
		garbage = sscanf((*fp + *fp_count), "%s", temp_str);
		len = strlen(temp_str);
		data -> doc_name = (char*) malloc((len + 1) * sizeof(char));
		(*fp_count) += len + 1;
        // while(*(*fp + *fp_count) != '\t' && *(*fp + *fp_count) != '\n'){
        //     (*fp_count) ++;
        // }
        // (*fp_count) ++;

		data -> frequency = frequency;
		strcpy(data -> doc_name, temp_str);

		list = add_to_end(list, data);
	}

	return list;
}