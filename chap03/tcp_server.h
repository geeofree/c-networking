#ifndef TCP_SERVER
#define TCP_SERVER
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "../utils/error.h"

/** @returns socketfd - Socket file descriptor. */
int setup_server(char *address, char *port, int backlog);

/** @returns connfd - Connection file descriptor. */
int accept_connection(int socketfd);

void close_connection(int socketfd);

void close_connections(int *fds, int fds_len);

#endif
