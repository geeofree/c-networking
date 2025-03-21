#include "tcp_client.h"

int connect_client(char *address, char *port) {
  struct addrinfo hints;
  struct addrinfo *client_address;
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;

  HANDLE_ERROR(getaddrinfo(address, port, &hints, &client_address), "getaddrinfo() failed.");

  int socketfd = HANDLE_ERROR(socket(client_address->ai_family, client_address->ai_socktype, client_address->ai_protocol), "socket() failed.");

  HANDLE_ERROR(connect(socketfd, client_address->ai_addr, client_address->ai_addrlen), "connect() failed.");
  freeaddrinfo(client_address);

  return socketfd;
}
