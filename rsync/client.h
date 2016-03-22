#ifndef __CLIENT_H__
#define __CLIENT_H__


#include "socket.h"

typedef struct {
  socket_t skt;  // Server socket
  const char* hostname;
  int port;
  const char* old_local_file;
  const char* new_local_file;
  const char* remote_file;
  int block_size;
} client_t;

int client_create(client_t* c, const char* hostname, int port,
                  const char* old_local_file, const char* new_local_file,
                  const char* remote_file, int block_size);
int client_run(client_t* c);
int client_destroy(client_t* c);

#endif
