#include "tcp.h"

int setup_server(char *address, char *port, int backlog) {
  struct addrinfo hint;
  memset(&hint, 0, sizeof(hint));
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE;

  struct addrinfo *server_address;
  HANDLE_ERROR(getaddrinfo(address, port, &hint, &server_address), "getaddrinfo() failed.");

  int socketfd = HANDLE_ERROR(socket(server_address->ai_family, server_address->ai_socktype, server_address->ai_protocol), "socket() failed.");

  HANDLE_ERROR(bind(socketfd, server_address->ai_addr, server_address->ai_addrlen), "bind() failed.");
  freeaddrinfo(server_address);

  HANDLE_ERROR(listen(socketfd, backlog), "listen() failed.");

  return socketfd;
}

int accept_connection(int socketfd) {
  struct sockaddr socket_address;
  socklen_t socket_address_len = sizeof(socket_address);
  int connfd = HANDLE_ERROR(accept(socketfd, &socket_address, &socket_address_len), "accept() failed.");
  return connfd;
}

void close_connections(int *fds, int fds_len) {
  int i;
  for (i = 0; i < fds_len; i++) {
    shutdown(fds[i], SHUT_RDWR);
    close(fds[i]);
  }
}
