#ifndef COMMONS_H
#define COMMONS_H

void handle_connection(int client_fd);
int start_server(int port, unsigned int backlog);

#endif