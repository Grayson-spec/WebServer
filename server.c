#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define PORT 8080

// Create and bind a socket to a specific port
int create_socket() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return -1;
    }
    return server_fd;
}

int bind_socket(int server_fd, int port) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd); // Close file descriptor on error
        return -1;
    }
    return 0;
}

int create_and_bind_socket(int port) {
    int server_fd = create_socket();
    if (server_fd < 0) {
        return -1;
    }

    if (bind_socket(server_fd, port) < 0) {
        close(server_fd); // Close file descriptor on error
        return -1;
    }

    return server_fd;
}

// Listen for incoming connections and accept them
int listen_and_accept(int server_fd) {
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(1);
    }

    // Accept incoming connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept");
        exit(1);
    }

    return client_fd;
}

void* handle_client_thread(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);

    // Handle client request
    handle_client(client_fd);
    close(client_fd);

    return NULL;
}

// handles requests
void handle_requests(int server_fd) {
    int client_fd;
    int connection_count = 0;

    while (1)
    {
        client_fd = listen_and_accept(server_fd);
        if (client_fd < 0)
        {
            perror("listen_and_accept");
            continue;
        }
        connection_count++;
        printf("Connection accepted from client IP address...\n");

        int* client_fd_ptr = malloc(sizeof(int));
        *client_fd_ptr = client_fd;

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client_thread, client_fd_ptr);
    }
    
}

// Start the server and handle incoming requests
int start_server(int port) {
    int server_fd = create_and_bind_socket(port);
    if (server_fd < 0) {
        return -1;  // error code
    }

    printf("Server listening on port %d...\n", port);
    handle_requests(server_fd);
    close(server_fd);
    return 0;
}