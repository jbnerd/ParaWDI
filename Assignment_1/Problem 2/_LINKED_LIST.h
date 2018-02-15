#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef _MY_LINKED_LIST
#define _MY_LINKED_LIST

struct Element{
	int frequency;
	char* doc_name;
};
typedef struct Element Element;

struct Node{
	Element* ele;
	struct Node* next;
};
typedef struct Node Node;

struct List{
	Node* head;
	Node* tail;
	unsigned int size;
};
typedef struct List List;

List* create_list(void);

Node* create_node(Element* data);

bool is_empty(List* list);

List* insert_in_order(List* list, Element* data);

#endif