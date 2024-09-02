#include <commons/protocol.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void handle_connection(int client_fd) {
  char buffer[4096] = {0};
  ProtocolHeader *header = buffer;
  header->type = htonl(PROTO_HELLO);
  int real_length = sizeof(int);
  header->length = htons(real_length);

  int *data = (int *)(&header[1]);
  *data = htonl(1);

  write(client_fd, header, sizeof(ProtocolHeader) + real_length);
}

int run_sync(int port) {
  struct sockaddr_in server_info = {0};
  server_info.sin_family = PF_INET;
  server_info.sin_addr.s_addr = 0;
  server_info.sin_port = htons(port);

  int server_fd = socket(PF_INET, SOCK_STREAM, 0);

  if (server_fd == -1) {
    perror("socket");

    return 1;
  }

  if (bind(server_fd, (struct sockaddr *)&server_info, sizeof(server_info)) ==
      -1) {
    perror("bind");
    close(server_fd);

    return 1;
  }

  if (listen(server_fd, 0) == -1) {
    perror("listen");
    close(server_fd);

    return 1;
  }

  struct sockaddr_in client_info = {0};
  unsigned int client_size = 0;

  while (1) {
    int client_fd =
        accept(server_fd, (struct sockaddr *)&client_info, &client_size);

    printf("Processing client fd: %d\n", client_fd);
    fflush(stdout);

    sleep(5);

    if (client_fd == -1) {
      perror("accept");
      close(server_fd);

      return 1;
    }

    handle_connection(client_fd);

    close(client_fd);
  }

  close(server_fd);

  return 0;
}
