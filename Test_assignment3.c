#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define PATTERN "happy"

void send_book_data(const char* filename) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Could not open file");
        return;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        fclose(file);
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        fclose(file);
        return;
    }

    // Read from file and send data to the server
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        send(sock, buffer, strlen(buffer), 0);
    }

    // Close the file and socket
    fclose(file);
    close(sock);
}

int main() {
    // Simulate sending book data from book1.txt to book10.txt
    for (int i = 1; i <= 10; i++) {
        char filename[20];
        snprintf(filename, sizeof(filename), "book%d.txt", i);
        send_book_data(filename);
        sleep(1); // Adding delay between sends
    }

    return 0;
}
