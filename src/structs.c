#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef utils
#include "utils.c"
#define utils
#endif

/* -- structs -- */

typedef struct node{
	char* value;
	int length;
	struct node* next;
}Node;

typedef struct registry{

	int offset;
	int length;
	struct registry* next;

}Register;

typedef struct header{
	int quantity;
	struct node* fields;
	struct registry* registers;
}Header;

/* -- signatures -- */

Node* NewNode();
Header* NewHeader();
Register* NewRegister();
void Push(Node * init, char* val);
void PushReg(Register* init, int offset, int length);

/* -- Initializers -- */

Node* NewNode(){
	Node* node =  malloc(sizeof(Node));
	node->next = NULL;
	return node;
}

Register* NewRegister(){
	Register* registry =  malloc(sizeof(Register));
	registry->next = NULL;
	return registry;
}

Header* NewHeader(){
	return (Header*) malloc(sizeof(Header));
}

/* -- functions -- */

void Push(Node* init, char* value) {
    Node * current = init;
    
    while (current->next != NULL)
        current = current->next;
   
    current->next = NewNode();
    current->value = value;
    current->length = GetCharSize(value);
}

void PushReg(Register* init, int offset, int length) {
    Register * current = init;
    
    while (current->next != NULL)
        current = current->next;
   
    current->next = NewRegister();
    current->offset = offset;
    current->length = length;
}