#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#define PORT 12345
#define MAX_CONNECTIONS 10 
#define BUFFER_SIZE 1024 





typedef struct Node{
  char* line;
  struct Node* next;
  struct Node* book_next;
}Node;


typedef struct SharedList{
  Node* head;
  Node* tail;
  pthread_mutex_t lock;
}SharedList;


SharedList shared_list;
int connection_counter = 0;


void init_shared_list(SharedList* list){
  list -> head = NULL;
  list -> tail = NULL;
  pthread_mutex_init(&(list -> lock), NULL);
}


void add_node(SharedList* list, char* line){
    Node *new_node = malloc(sizeof(Node));
    new_node->line = strdup(line);
    new_node->next = NULL;
    new_node->book_next = NULL;

    pthread_mutex_lock(&list->lock);
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    printf("Added node: %s", line);
    pthread_mutex_unlock(&list->lock);
}


void *handle_client(void *client_socket) {
    int sock = *(int *)client_socket;
    char buffer[BUFFER_SIZE];
    char *book_lines[MAX_CONNECTIONS];
    int book_line_count = 0;

    while (1) {
        ssize_t bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) break;

        buffer[bytes_received] = '\0';
        char *line = strtok(buffer, "\n");
        while (line != NULL) {
            add_node(&shared_list, line);
            book_lines[book_line_count++] = strdup(line);
            line = strtok(NULL, "\n");
        }
    }

    char filename[30];
    snprintf(filename, sizeof(filename), "book_%02d.txt", connection_counter++);
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < book_line_count; i++) {
        fprintf(file, "%s\n", book_lines[i]);
        free(book_lines[i]);
    }
    fclose(file);
    printf("Saved %s with %d lines.\n", filename, book_line_count);
    close(sock);
    free(client_socket);
    return NULL;
}




int main(int argc, char* argv[]){
  int server_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);

  init_shared_list(&shared_list);init_shared_list(&shared_list);

  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }


  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind (server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
    perror("Bind failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }


  if ((listen(server_socket, MAX_CONNECTIONS)) < 0){
    perror("Listening failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);
  while (1){
    int* client_socket = malloc(sizeof(int));
    *client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if(*client_socket < 0){
      perror("Accepting failed");
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
