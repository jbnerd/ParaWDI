#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef _MY_LINKED_LIST
#define _MY_LINKED_LIST

struct Element{
	unsigned int frequency;
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

List* add_to_end(List* list, Element* data);

List* mergeLists(List* list1, List* list2);

List* delete_from_end(List* list);

void print_list(List* list);

List* delete_from_front(List* list);

void serialize_list(List* list, FILE** fp);

List* deserialize_list(FILE** fp);

#endif
