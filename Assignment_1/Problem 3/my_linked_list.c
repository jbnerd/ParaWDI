#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_LINKED_LIST.h"

List* create_list(){
	List* temp = (List*) malloc(sizeof(List));
	if(temp == NULL){
		return NULL;
	}else{
		temp -> head = NULL;
		temp -> tail = NULL;
		temp -> size = 0;
		return temp;
	}
}

Node* create_node(Element* data){
	Node* temp = (Node*) malloc(sizeof(Node));
	if(temp == NULL){
		return NULL;
	}else{
		temp -> ele = data;
		temp -> next = NULL;
		return temp;
	}
}

List* insert_in_order(List* list, Element* data){
	Node* current;
	Node* new_node = create_node(data);
    if (list -> size == 0){
        list -> head = new_node;
        list -> tail = new_node;
        list -> size ++;
    }else if(list -> head -> ele -> frequency <= new_node -> ele -> frequency){
    	new_node -> next = list -> head;
    	list -> head = new_node;
    	list -> size ++;
    }else{
        current = list -> head;
        while (current -> next != NULL && current -> next -> ele -> frequency > new_node -> ele -> frequency){
            current = current -> next;
        }
        new_node -> next = current -> next;
        current -> next = new_node;
        if(current -> next == NULL){
        	list -> tail = new_node;
        }
        list -> size ++;
    }
}

List* add_to_end(List* list, Element* data){
    Node* new_node = create_node(data);
    if(list -> size == 0){
        list -> head = new_node;
        list -> tail = new_node;
        list -> size ++;
    }else{
        list -> tail -> next = new_node;
        list -> tail = new_node;
        list -> size ++;
    }
    return list;
}

List* delete_from_end(List* list){
	Node* del_node = list -> tail;
	Node* temp = list -> head;

	if(list -> size == 0){
		return NULL;
	}else if(list -> size == 1){
		list -> head = NULL;
		list -> tail = NULL;
		free(del_node);
	}else{
		while(temp -> next -> next != NULL){
			temp = temp -> next;
		}

		temp -> next = NULL;
		list -> tail = temp;
		free(del_node);
	}
	list -> size --;

	return list;
}

List* delete_from_front(List* list){
	Node* del_node = list -> head;

	if(list -> size == 0){
		return NULL;
	}else if(list -> size == 1){
		list -> head = NULL;
		list -> tail = NULL;
		free(del_node);
	}else{
		list -> head = list -> head -> next;
		free(del_node);
	}
	list -> size --;

	return list;
}

bool is_empty(List* list){
	if(list -> size == 0){
		return true;
	}else{
		return false;
	}
}

void print_list(List* list){
	Node* temp = list -> head;
	while(temp != NULL){
		printf("%d::%s\n", temp -> ele -> frequency, temp -> ele -> doc_name);
		temp = temp -> next;
	}
	printf("\n");
}

List* mergeLists(List* list1, List* list2){
    List* ret = create_list();
    Node* iter1 = list1 -> head;
    Node* iter2 = list2 -> head;

    while(iter1 != NULL && iter2 != NULL){
        if(iter1 -> ele -> frequency > iter2 -> ele -> frequency){
            if(!ret -> head){
                ret -> head = iter1;
                ret -> tail = iter1;
                iter1 = iter1 -> next;
                ret -> tail -> next = NULL;
            }else{
                ret -> tail -> next = iter1;
                ret -> tail = iter1;
                iter1 = iter1 -> next;
                ret -> tail -> next = NULL;
            }
        }else{
            if(!ret -> head){
                ret -> head = iter2;
                ret -> tail = iter2;
                iter2 = iter2 -> next;
                ret -> tail -> next = NULL;
            }else{
                ret -> tail -> next = iter2;
                ret -> tail = iter2;
                iter2 = iter2 -> next;
                ret -> tail -> next = NULL;
            }
        }
    }

    if(iter1 == NULL){
        if(list1 -> size == 0){
            ret -> head = iter2;
            ret -> tail = list1 -> tail;
        }else{
            ret -> tail -> next = iter2;
            while(ret -> tail -> next != NULL){
                ret -> tail = ret -> tail -> next;
            }
        }
    }

    if(iter2 == NULL){
        if(list2 -> size == 0){
            ret -> head = iter1;
            ret -> tail = list2 -> tail;
        }else{
            ret -> tail -> next = iter1;
            while(ret -> tail -> next != NULL){
                ret -> tail = ret -> tail -> next;  
            }
        }
    }

    return ret;
}

List* copy_list(List* list){
    List* ret_list = create_list();
    Node* temp = list -> head;
    while(temp != NULL){
        Element* data_temp = (Element*) malloc(sizeof(Element));
        data_temp -> frequency = temp -> ele -> frequency;
        if(temp -> ele -> doc_name){
            int len = strlen(temp -> ele -> doc_name);
            data_temp -> doc_name = (char*) malloc((len + 1) * sizeof(char));
            data_temp -> doc_name = memset(data_temp -> doc_name, '\0', (len + 1));
            data_temp -> doc_name = memcpy(data_temp -> doc_name, temp -> ele -> doc_name, (len));
        }else{
            data_temp -> doc_name = NULL;
        }
        ret_list = insert_in_order(ret_list, data_temp);
        temp = temp -> next;
    }
    return ret_list;
}


// int main(){

// 	Element* data = (Element*) malloc(sizeof(Element));
// 	Element* data1 = (Element*) malloc(sizeof(Element));
// 	Element* data2 = (Element*) malloc(sizeof(Element));

//     Element* data3 = (Element*) malloc(sizeof(Element));
// 	Element* data4 = (Element*) malloc(sizeof(Element));
// 	Element* data5 = (Element*) malloc(sizeof(Element));

// 	data -> frequency = 9;
// 	data1 -> frequency = 5;
// 	data2 -> frequency = 2;
//     data3 -> frequency = 1;
// 	data4 -> frequency = 7;
// 	data5 -> frequency = 3;   

//     data -> doc_name = (char*) malloc(4 * sizeof(char));
//     data -> doc_name = "lel";

// 	List* list = create_list();
// 	list = insert_in_order(list, data);
// 	list = insert_in_order(list, data1);
// 	list = insert_in_order(list, data2);
//     list = insert_in_order(list, data3);
//     list = insert_in_order(list, data4);
// 	print_list(list);

//     List* list2 = copy_list(list);
// 	// list2 = insert_in_order(list2, data3);
// 	// list2 = insert_in_order(list2, data4);
// 	// list2 = insert_in_order(list2, data5);
// 	print_list(list2);

//     // List* l3 = mergeLists(list, list2); 
//     // print_list(l3);
// 	return 0;
// }
