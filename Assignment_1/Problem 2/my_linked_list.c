#include <stdio.h>
#include <stdlib.h>
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
    if (list -> size == 0 || list -> head -> ele -> frequency <= new_node -> ele -> frequency){
        new_node -> next = list -> head;
        list -> head = new_node;
        list -> tail = new_node;
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
    }
}

bool is_empty(List* list){
	if(list -> size == 0){
		return true;
	}else{
		return false;
	}
}