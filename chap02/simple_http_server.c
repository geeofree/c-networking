#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define SERVER_PORT "1234"
#define LISTEN_BACKLOG 10
#define check(return_val, error_message) (handle_error(return_val, error_message, __LINE__))
#define BUFFER_LEN 1024

int setup_server(char *, int);
int handle_error(int, char *, int);
int accept_connection(int);
void handle_connection(int);
void close_connections(int *, int);

int main() {
  printf("Setting up server...\n");
  int socketfd = setup_server(SERVER_PORT, LISTEN_BACKLOG);
  printf("Accepting connections...\n");
  int new_connection_socketfd = accept_connection(socketfd);
  handle_connection(new_connection_socketfd);
  int connections[] = { socketfd, new_connection_socketfd };
  printf("Closing connections...\n");
  close_connections(connections, sizeof(connections));
  printf("Finished...\n");
  return EXIT_SUCCESS;
}

int setup_server(char *port, int backlog) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *bind_address;

  check(
    getaddrinfo(NULL, port, &hints, &bind_address),
    "getaddrinfo() failed."
  );

  int sockfd = check(
    socket(
      bind_address->ai_family,
      bind_address->ai_socktype,
      bind_address->ai_protocol
    ),
    "socket() failed."
  );

  check(bind(sockfd, bind_address->ai_addr, bind_address->ai_addrlen), "bind() failed.");
  freeaddrinfo(bind_address);

  check(listen(sockfd, backlog), "listen() failed.");

  return sockfd;
}

int accept_connection(int socketfd) {
  struct sockaddr socket_address;
  socklen_t socket_address_len = sizeof(socket_address);
  int new_connection_socketfd = accept(socketfd, &socket_address, &socket_address_len);
  return new_connection_socketfd;
}

void handle_connection(int connection_socketfd) {
  char request[BUFFER_LEN];
  ssize_t bytes_received = check(recv(connection_socketfd, request, sizeof(request), 0), "recv() failed.");

  printf("Received bytes: %zd\n", bytes_received);
  printf("%.*s", (int)bytes_received, request);

  char *response = "HTTP/1.1 200 OK\r\n"
    "\r\n"
    "hello there, friend!\r\n";
  ssize_t bytes_sent = check(send(connection_socketfd, response, strlen(response), 0), "send() failed.");
  printf("Sent bytes: %zd\n", bytes_sent);
}

void close_connections(int *connections, int connection_len) {
  int i;
  for (i = 0; i < connection_len; i++) {
    shutdown(connections[i], SHUT_RDWR);
    close(connections[i]);
  }
}

int handle_error(int return_val, char *error_message, int line_no) {
  if (return_val < 0) {
    fprintf(
      stderr,
      "%s (line: %d, errno: %d, error: %s)\n",
      error_message,
      line_no,
      errno,
      strerror(errno)
    );
    exit(EXIT_FAILURE);
  }
  return return_val;
}
