
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h> //sacar

#include "socket.h"


#define NUM_CLIENTS 5


int socket_create(socket_t* s) {
  s->fd = socket(AF_INET, SOCK_STREAM, 0);
  return s->fd;
}


int socket_bind_and_listen(socket_t* s, int port) {
  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  int c = bind(s->fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (c < 0) return c;

  return listen(s->fd, NUM_CLIENTS);
}


socket_t socket_accept(socket_t* s) {
  struct sockaddr_in cli_addr;
  socket_t cli_socket;
  socklen_t cli_len = sizeof(cli_addr);
  cli_socket.fd = accept(s->fd, (struct sockaddr *) &cli_addr, &cli_len);

  return cli_socket;
}


int socket_connect(socket_t* s, const char* hostname, int port) {
  struct hostent *server;
  struct sockaddr_in serv_addr;

  server = gethostbyname(hostname);
  if (server == NULL) return 1;

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
       (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);
  serv_addr.sin_port = htons(port);

  return connect(s->fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
}


int socket_read(socket_t* s, char* buff, size_t bytes) {
  int length = bytes;
  while (length > 0)
  {
    // There are no MSG_NOSIGNAL on OS X, so i pass no flags
    int i = recv(s->fd, buff, length, 0);
    if (i < 1) return i;
    buff += i;
    length -= i;
  }
  return 0;
}


int socket_write(socket_t* s, char* buff, size_t bytes) {
  int length = bytes;
  while (length > 0)
  {
    // There are no MSG_NOSIGNAL on OS X, so i pass no flags
    int i = send(s->fd, buff, length, 0);
    if (i < 1) return i;
    buff += i;
    length -= i;
  }
  return 0;
}


int socket_destroy(socket_t* s) {
  return close(s->fd);
}
