#include "tcp_server.h"
#include "tcp_client.h"

#define LISTEN_BACKLOG 10
#define CHAR_BUFFER 4024
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

void handle_tcp_server(char *address, char *port);
void handle_tcp_client(char *address, char *port);

int main(int argc, char **argv) {
  if (argc < 4) {
    printf("Usage: %s <str:client|server> <str:address> <int:port>\n", argv[0]);
    return EXIT_SUCCESS;
  }

  if (strcmp(argv[1], "server") == 0) {
    handle_tcp_server(argv[2], argv[3]);
    return EXIT_SUCCESS;
  }

  if (strcmp(argv[1], "client") == 0) {
    handle_tcp_client(argv[2], argv[3]);
    return EXIT_SUCCESS;
  }

  fprintf(stderr, "Invalid TCP type. Should be 'client' or 'server', received '%s'\n", argv[1]);
  fprintf(stderr, "Usage: %s <str:client|server> <str:address> <int:port>\n", argv[0]);
  return EXIT_FAILURE;
}

void handle_tcp_server(char *address, char *port) {
  int socketfd = setup_server(address, port, LISTEN_BACKLOG);

  fd_set socket_set;
  FD_ZERO(&socket_set);
  FD_SET(socketfd, &socket_set);

  int max_socket = socketfd;

  while (1) {
    fd_set ready_sockets;
    ready_sockets = socket_set;

    HANDLE_ERROR(select(max_socket + 1, &ready_sockets, NULL, NULL, NULL), "select() failed.");

    int i;
    for (i = 1; i <= max_socket; i++) {
      if (FD_ISSET(i, &ready_sockets)) {
        if (i == socketfd) {
          printf("New connection\n");
          int connfd = accept_connection(i);
          FD_SET(connfd, &socket_set);
          if (max_socket < connfd) {
            max_socket = connfd;
          }
        } else {
          char request[CHAR_BUFFER];
          ssize_t bytes_received = HANDLE_ERROR(recv(i, request, CHAR_BUFFER, 0), "recv() failed");

          if (bytes_received == 0) {
            printf("A connection has closed\n");
            FD_CLR(i, &socket_set);
            close_connection(i);
            continue;
          }

          printf("<socket: %d; bytes received: %zd>%.*s\n", i, bytes_received, (int)bytes_received, request);
          int j;
          for (j = 1; j <= max_socket; j++) {
            if (FD_ISSET(j, &socket_set)) {
              if (i == j || j == socketfd) {
                continue;
              } else {
                ssize_t bytes_sent = HANDLE_ERROR(send(j, request, bytes_received, 0), "send() failed.");
              }
            }
          }
        }
      }
    }
  }

  close_connection(socketfd);
}

void handle_tcp_client(char *address, char *port) {
  int socketfd = connect_client(address, port);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 100000;

  while (1) {
    fd_set socket_set;
    FD_ZERO(&socket_set);

    FD_SET(socketfd, &socket_set);
    FD_SET(STDIN_FILENO, &socket_set);

    int max_socket = socketfd;
    if (STDIN_FILENO > socketfd) {
      max_socket = STDIN_FILENO;
    }

    HANDLE_ERROR(select(max_socket + 1, &socket_set, NULL, NULL, &timeout), "select() failed.");

    if (FD_ISSET(socketfd, &socket_set)) {
      char request[CHAR_BUFFER];
      ssize_t bytes_received = HANDLE_ERROR(recv(socketfd, request, CHAR_BUFFER, 0), "recv() failed");

      if (bytes_received == 0) {
        printf("Peer closed\n");
        continue;
      }

      printf("<Bytes received: %zd> %.*s", bytes_received, (int)bytes_received, request);
    }

    if (FD_ISSET(STDIN_FILENO, &socket_set)) {
      char sent[CHAR_BUFFER];
      if (!fgets(sent, CHAR_BUFFER, stdin)) break;
      ssize_t bytes_sent = HANDLE_ERROR(send(socketfd, sent, strlen(sent), 0), "send() failed.");
    }
  }

  close_connection(socketfd);
}
