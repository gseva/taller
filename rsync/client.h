#ifndef __CLIENT_H__
#define __CLIENT_H__

/**
 * Abstracción del cliente.
 */

#include "socket.h"

typedef struct {
  socket_t skt;  // Server socket
  const char* hostname;
  const char* port;
  const char* old_local_file;
  const char* new_local_file;
  const char* remote_file;
  int block_size;
} client_t;


/**
 * Creación del cliente. Necesita de un destroy posteriormente.
 *
 * @param  c              Puntero al cliente
 * @param  hostname       Hostname del servidor, ej: 127.0.0.1
 * @param  port           Puerto del servidor
 * @param  old_local_file La ruta al archivo que se desea actualizar
 * @param  new_local_file La ruta al archivo donde guardar el resultado
 * @param  remote_file    La ruta al archivo remoto en el servidor
 * @param  block_size     Tamaño del bloque a sacar checksums
 * @return                Distinto de 0 si hay un error.
 */
int client_create(client_t* c, const char* hostname, const char* port,
                  const char* old_local_file, const char* new_local_file,
                  const char* remote_file, int block_size);

/**
 * Realiza la comunicación con el servidor y la actualziación del archivo.
 */
int client_run(client_t* c);

/**
 * Libera los recursos que tomó la creación del cliente.
 */
int client_destroy(client_t* c);

#endif
