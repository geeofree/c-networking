#ifndef TCP_SELECT_CLIENT_H
#define TCP_SELECT_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sys/select.h>

#define READ_BUFFER 4096

/**
 * @returns socketfd - Socket file descriptor.
 **/
int setup_client(char *address, char *port);
void handle_connection(int socketfd);
void close_connection(int socketfd);

#endif
