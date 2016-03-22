#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>

#include "bag.h"
#include "socket.h"


typedef struct {
  socket_t skt;  // Accept socket
  socket_t c_skt;  // Client socket
  int port;
  char* new_file;
  int block_size;
  bag_t checksums;
} server_t;

int server_create(server_t* s, int port);
int server_run(server_t* s);
int server_destroy(server_t* s);

#endif
