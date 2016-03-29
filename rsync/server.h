#ifndef __SERVER_H__
#define __SERVER_H__

/**
 * Abstracción del servidor
 */

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

/**
 * Creación del servidor. Necesita de un posterior destroy.
 *
 * @param  s    Puntero al servidor
 * @param  port Puerto donde correrá el servidor
 * @return      Distinto de 0 si hubo algún error
 */
int server_create(server_t* s, int port);

/**
 * Realiza la comunicación con el cliente. Espera la conección del cliente y
 * envia la información necesaria.
 */
int server_run(server_t* s);

/**
 * Libera los recursos tomados por la creación del servidor.
 */
int server_destroy(server_t* s);

#endif
