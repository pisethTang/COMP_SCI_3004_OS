#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <pthread.h>


extern pthread_mutex_t lock;
void initialize_list();
void add_node(const char *data);
void write_book_to_file(const char *filename);

#endif
