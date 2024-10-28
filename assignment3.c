#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t lock;
pthread_mutex_t book_lock;
int book_count = 0;

struct Node {
    char *data;
    struct Node *next;
    struct Node *book_next;
};

struct NodeList {
    struct Node *list_head;
    struct Node *book_head;
};

struct Node *create_node(const char *data) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->data = strdup(data);
    new_node->next = NULL;
    new_node->book_next = NULL;
    return new_node;
}

void add_node(struct NodeList *nodes, const char *data) {
    struct Node *new_node = create_node(data);
    pthread_mutex_lock(&lock);

    // Add to the list
    if (nodes->list_head == NULL) {
        nodes->list_head = new_node;
    } else {
        struct Node *current = nodes->list_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }

    // Add to the book list
    if (nodes->book_head == NULL) {
        nodes->book_head = new_node;
    } else {
        struct Node *book_current = nodes->book_head;
        while (book_current->book_next != NULL) {
            book_current = book_current->book_next;
        }
        book_current->book_next = new_node;
    }

    pthread_mutex_unlock(&lock);
}

void *copy_text(void *arg) {
    int socket = *(int *)arg;
    free(arg);
    char buffer[2048];

    // Allocate and initialize NodeList
    struct NodeList *nodes = (struct NodeList *)malloc(sizeof(struct NodeList));
    nodes->list_head = NULL;
    nodes->book_head = NULL;

    struct timeval timeout = {1, 0}; // 1 second timeout
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    while (1) {
        bzero(buffer, sizeof(buffer));
        int text_length = recv(socket, buffer, sizeof(buffer) - 1, 0);
        if (text_length <= 0) break; // Exit loop on disconnect or error

        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character if exists
        add_node(nodes, buffer); // Add received data to the node list
        printf("Added node with data: %s\n", buffer);
    }

    // File creation upon client disconnection
    pthread_mutex_lock(&book_lock);
    book_count++;
    char book_name[1024];
    snprintf(book_name, sizeof(book_name), "book_%02d.txt", book_count);

    FILE *new_book = fopen(book_name, "w");
    if (new_book == NULL) {
        perror("Error opening file");
        pthread_mutex_unlock(&book_lock);
        free(nodes);
        close(socket);
        return NULL;
    }

    // Write all received data to the book file
    struct Node *current = nodes->book_head; // Use book_head to write the book
    while (current != NULL) {
        fprintf(new_book, "%s\n", current->data); // Write data with newline for readability
        current = current->book_next; // Traverse the book linked list
    }
    fclose(new_book);
    pthread_mutex_unlock(&book_lock);

    // Cleanup
    current = nodes->list_head;
    while (current) {
        struct Node *temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
    free(nodes);
    close(socket);
    return NULL;
}

int setup_server_socket(int argc, char *argv[]) {
    int port = 1234;
    int option;

    while ((option = getopt(argc, argv, "l:p")) != -1) {
        if (option == 'l') port = atoi(optarg);
        else if (option == 'p') printf("Process ID: %d\n", getpid());
        else {
            fprintf(stderr, "Usage: %s [-l port_number] [-p]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    return port;
}

int create_and_bind_socket(int port) {
    struct sockaddr_in serv_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) perror("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons((uint16_t)port);
    
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }
    listen(sockfd, 5);
    return sockfd;
}

void accept_and_handle_clients(int sockfd) {
    struct sockaddr_in cli_addr;
    while (1) {
        socklen_t clilen = sizeof(cli_addr);
        int *newsockfd = malloc(sizeof(int));
        *newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (*newsockfd < 0) {
            free(newsockfd);
            perror("ERROR on accept");
            continue;
        }
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, copy_text, (void *)newsockfd) != 0) {
            perror("Failed to create thread");
            free(newsockfd);
        }
        pthread_detach(thread_id);
    }
}

int main(int argc, char *argv[]) {
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&book_lock, NULL);

    int port = setup_server_socket(argc, argv);
    int sockfd = create_and_bind_socket(port);

    printf("Server is running on port %d\n", port);
    accept_and_handle_clients(sockfd);

    close(sockfd);
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&book_lock);
    return 0;
}
