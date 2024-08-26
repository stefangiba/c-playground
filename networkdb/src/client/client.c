#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5555

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <ip>\n", argv[0]);

    return 1;
  }

  int client_fd = socket(PF_INET, SOCK_STREAM, 0);

  if (client_fd == -1) {
    perror("socket");

    return 1;
  }

  struct sockaddr_in server_info = {0};
  server_info.sin_family = PF_INET;
  server_info.sin_addr.s_addr = inet_addr(argv[1]);
  server_info.sin_port = htons(PORT);

  if (connect(client_fd, (struct sockaddr *)&server_info,
              sizeof(server_info)) == -1) {
    perror("connect");
    close(client_fd);

    return 1;
  }

  close(client_fd);

  return 0;
}