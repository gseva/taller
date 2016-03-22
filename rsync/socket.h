#ifndef __SOCKET_H__
#define __SOCKET_H__



typedef struct {
  int fd;
} socket_t;


int socket_create(socket_t* s);

int socket_destroy(socket_t* s);

int socket_bind_and_listen(socket_t* s, int port);

socket_t socket_accept(socket_t* s);

int socket_connect(socket_t* s, const char* hostname, int port);

int socket_read(socket_t* s, char* buff, size_t bytes);

int socket_write(socket_t* s, char* buff, size_t bytes);

#endif
