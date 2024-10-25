#include "LinkedList.h"



struct Node_int* create_node_int(void);
void destroy_node_int(struct Node_int* node_to_destroy);

struct Node_int* iterate(int index, struct LinkedList_int* linked_list);

// Instead, define functions that we are going to use in the struct 
void insert_node_int(int index, int data, struct LinkedList_int* linked_list);
void remove_node_int(int index, struct LinkedList_int* linked_list);
int retrieve_data_int(int index, struct LinkedList_int* linked_list);


struct LinkedList_int linked_list_int_constructor()
{
    // Define a new linked list object
    struct LinkedList_int new_list;
    new_list.head = NULL;
    new_list.length = 0;

    // Instantiate some of the members
    new_list.insert = insert_node_int;
    new_list.remove = remove_node_int;
    new_list.retrieve = retrieve_data_int;
    new_list.constructor = linked_list_int_constructor();


    // Send the list back to the user. 
    return new_list;
}







