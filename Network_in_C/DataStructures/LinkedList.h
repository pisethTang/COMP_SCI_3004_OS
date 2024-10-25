#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "Node.h"
#include <stdlib.h>
// Define a structure for the linked list node



struct LinkedList_int{
    struct Node_int* head;
    int length;

    // Defining function pointers. We can rename the function pointers for better user experience. 
    // Observe that the type of a function pointer has to be the same as the return type of the function. 
    void (*insert)(int index, int data, struct LinkedList_int* linked_list);
    void (*remove)(int index, int data, struct LinkedList_int* linked_list);
    int (*retrieve)(int index, int data, struct LinkedList_int* linked_list);
    struct LinkedList_int (*constructor)(void);
};


// struct NodeInt* iterate(int index, struct* LinkedList_int linked_list);

struct LinkedList_int* linked_list_constructor();


#endif /*LINKED_LIST_H*/