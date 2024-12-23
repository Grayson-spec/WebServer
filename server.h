#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

extern void start_server(int port);
void handle_client(int client_fd);

#endif  // SERVER_H