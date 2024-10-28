#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>





struct Node {
    char *data;
    struct Node *next;
    struct Node *book_next;
};




struct Node *list_head = NULL;
struct Node *book_head = NULL;

void initialize_list() {
    pthread_mutex_init(&lock, NULL);
    list_head = NULL;
    book_head = NULL;
}

struct Node *create_node(const char *data) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->data = strdup(data);
    new_node->next = NULL;
    new_node->book_next = NULL;
    return new_node;
}

void add_node(const char *data) {
    struct Node *new_node = create_node(data);
    pthread_mutex_lock(&lock);

    if (list_head == NULL) {
        list_head = new_node;
    } else {
        struct Node *current = list_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }

    if (book_head == NULL) {
        book_head = new_node;
    } else {
        struct Node *book_current = book_head;
        while (book_current->book_next != NULL) {
            book_current = book_current->book_next;
        }
        book_current->book_next = new_node;
    }

    pthread_mutex_unlock(&lock);
}

void write_book_to_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;

    pthread_mutex_lock(&lock);
    struct Node *current = book_head;
    while (current != NULL) {
        fprintf(file, "%s", current->data);
        current = current->book_next;
    }
    pthread_mutex_unlock(&lock);

    fclose(file);
}























