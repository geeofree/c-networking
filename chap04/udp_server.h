#ifndef UDP_SERVER_H
#define UDP_SERVER_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "../utils/error.h"

int udp_setup_server(char *address, char *port, struct addrinfo **address_info);
void close_connection(int fd);
#endif
