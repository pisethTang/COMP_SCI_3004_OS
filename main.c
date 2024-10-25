// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <pthread.h>
#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>


#define PORT 12345 
#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024

// Creating a multi-threaded server connection

typedef struct Node{
    char* line;
    struct Node* next;
    struct Node* book_next;
} Node;


typedef struct SharedList{
    Node* head;
    Node* tail;
    pthread_mutex_t lock;
}SharedList;


SharedList shared_list;
int connection_counter = 0;



void init_shared_list(SharedList* list){}

void add_node(SharedList* list, char* line){}

void* handle_client(void* client_socket){
    return 0;
}


int main(int argc, char* argv[]){
    // Declaring socket variables to grab client information
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    init_shared_list(&shared_list);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);


    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CONNECTIONS) < 0){
        perror("Listen failed!");
        close(server_socket);
        exit(EXIT_FAILURE);
    }



    printf("Server listening on port %d\n", PORT);
    while(1){
        int* client_socket = malloc(sizeof(int));

        *client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_len);

        if (*client_socket < 0){
            perror("Accept failed.");
            free(client_socket);
            continue;
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_socket);
        pthread_detach(tid);
    }

    close(server_socket);
    return 0;
}