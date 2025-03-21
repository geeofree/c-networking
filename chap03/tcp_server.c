#include "tcp_server.h"

int setup_server(char *address, char *port, int backlog) {
  struct addrinfo hints;
  struct addrinfo *server_address;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  HANDLE_ERROR(getaddrinfo(address, port, &hints, &server_address),"getaddrinfo() failed.");

  int socketfd = HANDLE_ERROR(socket(server_address->ai_family, server_address->ai_socktype, server_address->ai_protocol), "socket() failed.");

  HANDLE_ERROR(bind(socketfd, server_address->ai_addr, server_address->ai_addrlen), "bind() failed.");

  HANDLE_ERROR(listen(socketfd, backlog), "listen() failed.");

  return socketfd;
}

int accept_connection(int socketfd) {
  struct sockaddr socket_address;
  socklen_t socket_address_len = sizeof(socket_address);
  int connfd = HANDLE_ERROR(accept(socketfd, &socket_address, &socket_address_len), "accept() failed.");
  return connfd;
}

void close_connection(int socketfd) {
  shutdown(socketfd, SHUT_RDWR);
  close(socketfd);
}

void close_connections(int *fds, int fds_len) {
  int i;
  for (i = 0; i < fds_len; i++) {
    close_connection(fds[i]);
  }
}
