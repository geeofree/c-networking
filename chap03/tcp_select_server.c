#include "tcp_select_server.h"
#include "utils.h"

#define LISTEN_BACKLOG 10

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s <str:address> <int:port_number>\n", argv[0]);
    return EXIT_FAILURE;
  }
  int socketfd = setup_server(argv[1], argv[2], LISTEN_BACKLOG);
  accept_connections(socketfd);
  close_connection(socketfd);
}

int setup_server(char *address, char *port, int backlog) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *bind_address;
  check(getaddrinfo(address, port, &hints, &bind_address), "getaddrinfo() failed.");

  int socketfd = check(
    socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol),
    "socket() failed."
  );

  check(bind(socketfd, bind_address->ai_addr, bind_address->ai_addrlen), "bind() failed.");
  freeaddrinfo(bind_address);

  check(listen(socketfd, backlog), "listen() failed.");

  return socketfd;
}

void accept_connections(int socketfd) {
  fd_set master;
  FD_ZERO(&master);
  FD_SET(socketfd, &master);
  int max_socket = socketfd;

  while (1) {
    fd_set reads;
    reads = master;

    check(select(max_socket + 1, &reads, 0, 0, 0), "select() failed.");

    int i;
    for (i = 1; i <= max_socket; ++i) {
      if (FD_ISSET(i, &reads)) {
        if (i == socketfd) {
          struct sockaddr socket_address;
          socklen_t socket_address_len = sizeof(socket_address);
          int accepted_connfd = check(
            accept(socketfd, &socket_address, &socket_address_len),
            "accept() failed."
          );

          FD_SET(accepted_connfd, &master);

          if (accepted_connfd > max_socket) {
            max_socket = accepted_connfd;
          }
        } else {
          char read[1024];
          ssize_t bytes_received = check(recv(i, read, sizeof(read), 0), "recv() failed.");

          if (bytes_received < 1) {
            FD_CLR(i, &master);
            close_connection(i);
            continue;
          }

          int j;
          for (j = 1; j < max_socket; ++j) {
            if (FD_ISSET(j, &master)) {
              if (j == socketfd || j == i) {
                continue;
              } else {
                send(j, read, bytes_received, 0);
              }
            }
          }
        }
      }
    }
  }
}

void close_connection(int socketfd) {
  shutdown(socketfd, SHUT_RDWR);
  close(socketfd);
}
