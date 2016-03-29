#ifndef __SOCKET_H__
#define __SOCKET_H__

/**
 * Abtracción del socket para utilizarlo con funciones de más alto nivel.
 */

typedef struct {
  int fd;
} socket_t;


/**
 * La creación del socket. Requiere de un posterior destroy.
 */
int socket_create(socket_t* s);

/**
 * Libera los recursos tomados por la creación.
 */
int socket_destroy(socket_t* s);

/**
 * Pone un socket a escuchar el puerto pasado por parámetro.
 */
int socket_bind_and_listen(socket_t* s, int port);

/**
 * Acepta una conección entrante para el socket pasado.
 */
socket_t socket_accept(socket_t* s);

/**
 * Conecta el socket al hostname y puerto pasados por parámetro
 */
int socket_connect(socket_t* s, const char* hostname, const char* port);

/**
 * Lee del socket un número de bytes especificado y lo escribe en la cadena
 * buff.
 */
int socket_read(socket_t* s, char* buff, size_t bytes);

/**
 * Escribe en el socket un número de bytes especificado de la cadena buff.
 */
int socket_write(socket_t* s, char* buff, size_t bytes);

#endif
