#include <arpa/inet.h>
#include <commons/constants.h>
#include <commons/protocol.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5555

void handle_connection(int fd) {
  char buffer[sizeof(ProtocolHeader) + sizeof(int)] = {0};
  read(fd, buffer, sizeof(ProtocolHeader) + sizeof(int));

  ProtocolHeader *header = (ProtocolHeader *) buffer;
  header->type = ntohl(header->type);
  header->length = ntohs(header->length);

  int *data = (int *)&header[1];
  *data = ntohl(*data);

  if (header->type != PROTO_HELLO) {
    printf("Unknown protocol type: %d\n", header->type);
    return;
  }

  if (*data != 1) {
    printf("Protocol version mismatch!\n");
    return;
  }

  printf("Server connected, protocol v1.\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <ip>\n", argv[0]);

    return STATUS_ERROR;
  }

  int client_fd = socket(PF_INET, SOCK_STREAM, 0);

  if (client_fd == -1) {
    perror("socket");

    return STATUS_ERROR;
  }

  struct sockaddr_in server_info = {0};
  server_info.sin_family = PF_INET;
  server_info.sin_addr.s_addr = inet_addr(argv[1]);
  server_info.sin_port = htons(PORT);

  if (connect(client_fd, (struct sockaddr *)&server_info,
              sizeof(server_info)) == -1) {
    perror("connect");
    close(client_fd);

    return STATUS_ERROR;
  }

  write(client_fd, "hello", 6);

  handle_connection(client_fd);

  close(client_fd);

  return STATUS_SUCCESS;
}