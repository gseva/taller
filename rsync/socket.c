
#include "common.h"
#include "socket.h"


// Sin este define el SERCOM no compila.
// http://stackoverflow.com/questions/11405819/does-struct-hostent-have-a-field-h-addr
#define h_addr h_addr_list[0] /* for backward compatibility */

#define NUM_CLIENTS 5

#ifndef MSG_NOSIGNAL
// En Os X no existe el MSG_NOSIGNAL, entonces lo uso sin flags.
#define SOCKET_FLAGS 0
#else
#define SOCKET_FLAGS MSG_NOSIGNAL
#endif


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

// Por algun extraño motivo SERCOM no entiende el getaddrinfo (tira error
// de compilacion) - entonces no puedo usar esta versión del connect.

// int socket_connect(socket_t* s, const char* hostname, const char* port) {
//   struct addrinfo hints, *servinfo, *p;

//   memset(&hints, 0, sizeof(hints));
//   hints.ai_family = AF_UNSPEC;
//   hints.ai_socktype = SOCK_STREAM;

//   getaddrinfo(hostname, port, &hints, &servinfo);

//   for (p = servinfo; p != NULL; p = p->ai_next) {
//     if (connect(s->fd, p->ai_addr, p->ai_addrlen) != -1) {
//       break;
//     }
//   }

//   freeaddrinfo(servinfo);
//   return 0;
// }



// Ambas versiones de socket_connect pierden memoria en OS X según el valgrind.
// Parece ser un feature y no un bug:
// http://stackoverflow.com/questions/13229913/getaddrinfo-memory-leak

int socket_connect(socket_t* s, const char* hostname, const char* port) {
  struct hostent *server;
  struct sockaddr_in serv_addr;

  server = gethostbyname(hostname);
  if (server == NULL) return 1;

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
       (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);
  serv_addr.sin_port = htons(atoi(port));

  return connect(s->fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
}


int socket_read(socket_t* s, char* buff, size_t bytes) {
  int length = bytes;
  while (length > 0)
  {
    int i = recv(s->fd, buff, length, SOCKET_FLAGS);
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
    int i = send(s->fd, buff, length, SOCKET_FLAGS);
    if (i < 1) return i;
    buff += i;
    length -= i;
  }
  return 0;
}


int socket_destroy(socket_t* s) {
  return close(s->fd);
}
