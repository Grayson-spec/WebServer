#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 8080

// Create and bind a socket to a specific port
int create_and_bind_socket(int port) {
    int server_fd;
    struct sockaddr_in server_addr;

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(1);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Bind server socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
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

// Start the server and handle incoming requests
void start_server(int port) {
    int server_fd = create_and_bind_socket(port);
    int client_fd;

    printf("Server listening on port %d...\n", port);

    while (1) {
        client_fd = listen_and_accept(server_fd);
        printf("Connection accepted from client IP address...\n");

        // Handle client request
        handle_client(client_fd);
    }
}