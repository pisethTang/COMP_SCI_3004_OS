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

#define PORT 12345 // Default port number
#define BUFFER_SIZE 2048
#define MAX_CONNECTIONS 10

struct Node {
    char *data;
    struct Node *next;
};

struct NodeList {
    struct Node *list_head;
    pthread_mutex_t lock;
};

struct ServerData {
    struct NodeList *nodes;
    int connection_id;
};

// Function to create a new node
struct Node *create_node(const char *data) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->data = strdup(data);
    new_node->next = NULL;
    return new_node;
}

// Function to add a node to the shared list
void add_node(struct NodeList *nodes, const char *data) {
    struct Node *new_node = create_node(data);
    pthread_mutex_lock(&nodes->lock);

    // Add to the end of the shared list
    if (nodes->list_head == NULL) {
        nodes->list_head = new_node;
    } else {
        struct Node *current = nodes->list_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    
    printf("Added node: %s\n", data);
    pthread_mutex_unlock(&nodes->lock);
}

// Client handler function
void *handle_client(void *arg) {
    struct ServerData *server_data = (struct ServerData *)arg;
    int client_socket = server_data->connection_id;
    char buffer[BUFFER_SIZE];

    // Read data from the client and add to the shared list
    while (1) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            break; // Connection closed or error
        }
        buffer[bytes_received] = '\0'; // Null terminate the received string
        add_node(server_data->nodes, buffer); // Add to shared list
    }

    // Write the received book to a file
    char filename[20];
    snprintf(filename, sizeof(filename), "book_%02d.txt", server_data->connection_id);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file for writing");
        goto cleanup;
    }
    
    struct Node *current = server_data->nodes->list_head;
    while (current != NULL) {
        fprintf(file, "%s", current->data);
        current = current->next; // Move through the book
    }
    fclose(file);
    
    printf("Written to file: %s\n", filename);

cleanup:
    // Clean up
    close(client_socket);
    // Free all nodes in the node list
    current = server_data->nodes->list_head;
    while (current) {
        struct Node *temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
    free(server_data->nodes);
    free(server_data);
    return NULL;
}

// Main server function
int main(int argc, char *argv[]) {
    int listen_port = PORT;
    pthread_mutex_t global_lock; // Added to manage global server access
    pthread_mutex_init(&global_lock, NULL);
    
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(listen_port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_socket, MAX_CONNECTIONS) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("Server listening on port %d\n", listen_port);
    int connection_count = 0;

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        connection_count++;
        struct ServerData *server_data = (struct ServerData *)malloc(sizeof(struct ServerData));
        
        // Allocate a new NodeList for each connection
        server_data->nodes = (struct NodeList *)malloc(sizeof(struct NodeList));
        server_data->nodes->list_head = NULL;
        pthread_mutex_init(&server_data->nodes->lock, NULL); // Initialize lock for this node list
        server_data->connection_id = connection_count;

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, server_data);
        pthread_detach(thread); // Detach thread to allow for independent cleanup
    }

    close(server_socket);
    return 0;
}
