#ifndef TCP_SELECT_SERVER_H
#define TCP_SELECT_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sys/select.h>

/**
 * @returns socketfd - Socket file descriptor.
 **/
int setup_server(char *address, char *port, int backlog);

void accept_connections(int socketfd);

void close_connection(int socketfd);
#endif
