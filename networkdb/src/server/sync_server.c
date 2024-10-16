#include <commons/constants.h>
#include <netdb.h>
#include <server/commons.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int run_sync(int port, unsigned int backlog) {
  int server_fd = start_server(port, backlog);
  if (server_fd == STATUS_ERROR) {
    return STATUS_ERROR;
  }

  struct sockaddr_in client_info = {0};
  unsigned int client_size = 0;

  while (1) {
    int client_fd =
        accept(server_fd, (struct sockaddr *)&client_info, &client_size);

    if (client_fd == -1) {
      perror("accept");
      close(server_fd);

      return STATUS_ERROR;
    }

    handle_connection(client_fd);

    close(client_fd);
  }

  close(server_fd);

  return STATUS_SUCCESS;
}
