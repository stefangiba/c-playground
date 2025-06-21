#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "commons/constants.h"
#include "commons/protocol.h"

void handle_connection(int client_fd) {
  char buffer[sizeof(ProtocolHeader) + sizeof(int)] = {0};
  ProtocolHeader *header = (ProtocolHeader *)buffer;
  header->type = htonl(PROTO_HELLO);
  int real_length = sizeof(int);
  header->length = htons(real_length);

  int *version = (int *)(&header[1]);
  *version = htonl(1);

  write(client_fd, header, sizeof(ProtocolHeader) + real_length);
}

int start_server(int port, unsigned int backlog) {
  int opt = 1;
  struct sockaddr_in server_info = {0};
  server_info.sin_family = AF_INET;
  server_info.sin_addr.s_addr = INADDR_ANY;
  server_info.sin_port = htons(port);

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd == -1) {
    perror("socket");

    return STATUS_ERROR;
  }

  // for more useful information regarding SO_REUSEADDR, please see
  // https://stackoverflow.com/questions/14388706/how-do-so-reuseaddr-and-so-reuseport-differ
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) ==
      -1) {
    perror("setsockopt");
    close(server_fd);

    return STATUS_ERROR;
  }

  if (bind(server_fd, (struct sockaddr *)&server_info, sizeof(server_info)) ==
      -1) {
    perror("bind");
    close(server_fd);

    return STATUS_ERROR;
  }

  if (listen(server_fd, backlog) == -1) {
    perror("listen");
    close(server_fd);

    return STATUS_ERROR;
  }

  printf("Server is listening on port: %d!\n", port);

  return server_fd;
}