#include "udp_server.h"

int udp_setup_server(char *address, char *port, struct addrinfo **address_info) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  HANDLE_ERROR(getaddrinfo(address, port, &hints, address_info), "getaddrinfo() failed.");

  int socketfd = HANDLE_ERROR(socket((*address_info)->ai_family, (*address_info)->ai_socktype, (*address_info)->ai_protocol), "socket() failed.");

  HANDLE_ERROR(bind(socketfd, (*address_info)->ai_addr, (*address_info)->ai_addrlen), "bind() failed.");
  freeaddrinfo(*address_info);

  return socketfd;
}

void close_connection(int fd) {
  shutdown(fd, SHUT_RDWR);
  close(fd);
}
