#include <arpa/inet.h>
#include <commons/constants.h>
#include <netdb.h>
#include <poll.h>
#include <server/commons.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_CONNECTIONS 256
#define BUFF_SIZE 4096

typedef enum {
  CONNECTED,
  DISCONNECTED,
} ConnectionState;

typedef struct {
  int fd;
  ConnectionState state;
  char buffer[BUFF_SIZE];
} Client;

void init_clients(Client clients[], int num_clients) {
  for (int i = 0; i < num_clients; i++) {
    clients[i].fd = -1;
    clients[i].state = DISCONNECTED;
    memset(clients[i].buffer, '\0', BUFF_SIZE);
  }
}

int find_free_slot(Client clients[], int num_clients) {
  for (int i = 0; i < num_clients; i++) {
    if (clients[i].fd == -1) {
      return i;
    }
  }

  return -1;
}

int find_slot_by_fd(Client clients[], int num_clients, int fd) {
  for (int i = 0; i < num_clients; i++) {
    if (clients[i].fd == fd) {
      return i;
    }
  }

  return -1;
}

int select_impl(int port, unsigned int backlog) {
  int server_fd = start_server(port, backlog);
  if (server_fd == STATUS_ERROR) {
    return STATUS_ERROR;
  }

  Client clients[MAX_CONNECTIONS];
  init_clients(clients, MAX_CONNECTIONS);

  int free_slot;
  fd_set read_fds, write_fds;
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  while (true) {
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_SET(server_fd, &read_fds);

    for (int idx = 0; idx < MAX_CONNECTIONS; idx++) {
      Client *client = &clients[idx];
      if (client->fd != -1) {
        FD_SET(client->fd, &read_fds);
      }
    }

    if (select(FD_SETSIZE, &read_fds, &write_fds, NULL, NULL) == -1) {
      perror("select");
      close(server_fd);

      return STATUS_ERROR;
    }

    if (FD_ISSET(server_fd, &read_fds)) {
      int client_fd =
          accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

      if (client_fd == -1) {
        perror("accept");
        continue;
      }

      printf("New incoming connection from %s:%d\n",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

      free_slot = find_free_slot(clients, MAX_CONNECTIONS);
      if (free_slot == -1) {
        printf(
            "Maximum client connections reached! Closing incoming "
            "connection...\n");
        close(client_fd);
      } else {
        clients[free_slot].fd = client_fd;
        clients[free_slot].state = CONNECTED;
      }
    }

    for (int idx = 0; idx < MAX_CONNECTIONS; idx++) {
      Client *client = &clients[idx];

      if (client->fd == -1 || !FD_ISSET(client->fd, &read_fds)) {
        continue;
      }

      printf("Processing client %d\n", idx);

      ssize_t nread =
          read(client->fd, client->buffer, sizeof(client->buffer) - 1);

      printf("Read %zd bytes from client %d\n", nread, idx);

      if (nread <= 0) {
        close(client->fd);

        // Client clean-up
        client->fd = -1;
        client->state = DISCONNECTED;
        memset(&client->buffer, 0, BUFF_SIZE);
        printf("Client %d disconnected\n", idx);

        continue;
      }

      printf("Received message from client %d: %s\n", idx, client->buffer);

      handle_connection(client->fd);
    }
  }

  // Close client sockets
  for (int idx = 0; idx < MAX_CONNECTIONS; idx++) {
    if (clients[idx].fd != -1) {
      close(clients[idx].fd);
    }
  }

  // Close the server socket
  close(server_fd);

  return STATUS_SUCCESS;
}

int poll_impl(int port, unsigned int backlog) {
  int server_fd, conn_fd, free_slot;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);

  struct pollfd fds[MAX_CONNECTIONS + 1];

  server_fd = start_server(port, backlog);
  if (server_fd == STATUS_ERROR) {
    return STATUS_ERROR;
  }

  Client clients[MAX_CONNECTIONS];
  init_clients(clients, MAX_CONNECTIONS);

  memset(fds, 0, sizeof(fds));
  fds[0].fd = server_fd;
  fds[0].events = POLLIN;

  while (1) {
    int nfds = 1;
    for (int client_idx = 0; client_idx < MAX_CONNECTIONS; client_idx++) {
      Client *client = &clients[client_idx];

      if (client->fd == -1) {
        continue;
      }

      fds[nfds].fd = client->fd;  // offset by 1 for server_fd
      fds[nfds].events = POLLIN;
      nfds++;
    }

    // Wait for an event on one for the sockets
    int n_events = poll(fds, nfds, -1);  // -1 means no timeout
    if (n_events == -1) {
      perror("poll");
      return STATUS_ERROR;
    }

    // Check for new connections
    if (fds[0].revents & POLLIN) {
      conn_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
      if (conn_fd == -1) {
        perror("accept");
        continue;
      }

      printf("New incoming connection from %s:%d\n",
             inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

      free_slot = find_free_slot(clients, MAX_CONNECTIONS);
      if (free_slot == -1) {
        printf(
            "Maximum client connections reached! Closing incoming "
            "connection...\n");
        close(conn_fd);
      } else {
        clients[free_slot].fd = conn_fd;
        clients[free_slot].state = CONNECTED;
        printf("Slot %d has fd %d\n", free_slot, conn_fd);
      }

      n_events--;
    }

    // Check each client for read / write connectivity
    // start from 1 to skip the server_fd
    for (int i = 1; i < nfds && n_events > 0; i++) {
      if (!(fds[i].revents & POLLIN)) {
        continue;
      }

      n_events--;

      int client_idx = find_slot_by_fd(clients, MAX_CONNECTIONS, fds[i].fd);
      if (client_idx == -1) {
        printf("Client not found!\n");
        continue;
      }

      Client *client = &clients[client_idx];
      ssize_t bytes_read =
          read(client->fd, client->buffer, sizeof(client->buffer) - 1);

      if (bytes_read <= 0) {
        close(client->fd);

        // Client clean-up
        client->fd = -1;
        client->state = DISCONNECTED;
        memset(clients->buffer, 0, BUFF_SIZE);
        printf("Client %d disconnected\n", client_idx);

        continue;
      }

      printf("Received message from client %d: %s\n", client_idx,
             client->buffer);

      handle_connection(client->fd);
    }
  }

  // Close client sockets
  for (int idx = 0; idx < MAX_CONNECTIONS; idx++) {
    Client *client = &clients[idx];
    if (client->fd != -1) {
      close(client->fd);
    }
  }

  // Close the server socket
  close(server_fd);

  return STATUS_SUCCESS;
}

int run_sync_multiplexed(int port, unsigned int backlog) {
  return poll_impl(port, backlog);
}
