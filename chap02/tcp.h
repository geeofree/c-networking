#ifndef TCP_H
#define TCP_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "../utils/error.h"

/**
 * @returns socketfd - Socket file descriptor.
 **/
int setup_server(char *address, char *port, int backlog);

int accept_connection(int socketfd);

void close_connections(int *fds, int fds_len);
#endif
