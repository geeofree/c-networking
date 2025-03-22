#include "udp_client.h"

int udp_setup_client(char *address, char *port, struct addrinfo **address_info) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_DGRAM;

  HANDLE_ERROR(getaddrinfo(address, port, &hints, address_info), "getaddrinfo() failed.");

  int socketfd = HANDLE_ERROR(socket((*address_info)->ai_family, (*address_info)->ai_socktype, (*address_info)->ai_protocol), "socket() failed.");

  return socketfd;
}
