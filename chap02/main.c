#include "tcp.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define LISTEN_BACKLOG 10
#define CHAR_BUFF 1024

void handle_connection(int connfd);

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: %s <str:address> <int:port>\n", argv[0]);
    return EXIT_SUCCESS;
  }

  printf("Setting up server...\n");
  int socketfd = setup_server(argv[1], argv[2], LISTEN_BACKLOG);

  printf("Accepting connections...\n");
  int connfd = accept_connection(socketfd);

  printf("Connection being handled...\n");
  handle_connection(connfd);

  printf("Closing connections...\n");
  int fds[] = { connfd, socketfd };
  close_connections(fds, ARRAY_SIZE(fds));

  return EXIT_SUCCESS;
}

void handle_connection(int connfd) {
  char request[CHAR_BUFF];
  ssize_t bytes_received = HANDLE_ERROR(recv(connfd, request, CHAR_BUFF, 0), "recv() failed.");
  printf("Received bytes: %zd; Message: %.*s", bytes_received, (int)bytes_received, request);

  char *message = "HTTP/1.1 200 OK \r\n\r\n"
    "Hello world!\r\n";
  ssize_t bytes_sent = HANDLE_ERROR(send(connfd, message, strlen(message), 0), "send() failed.");
  printf("Sent bytes: %zd; Message: %s", bytes_sent, message);
}
