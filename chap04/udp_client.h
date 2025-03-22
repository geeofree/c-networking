#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "../utils/error.h"

int udp_setup_client(char *address, char *port, struct addrinfo **address_info);
#endif
