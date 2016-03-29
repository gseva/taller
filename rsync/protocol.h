#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/**
 * Abstracción del protocolo - las partes necesarias para que el cliente y
 * el servidor se puedan entender.
 *
 * Uso el prefijo P_ para las definiciones globales de flags para la
 * comunicación entre ellos.
 */

#define P_CHECKSUM_START 1
#define P_NO_MORE_CHECKSUMS 2

#define P_NEW_FILE_CHUNK 3
#define P_BLOCK_FOUND 4
#define P_END_OF_FILE 5


/**
 * Variante del checksum Adler32. El parámetro bytes define cuantos bytes
 * de la cadena de caracteres x se van a utilizar.
 */
int checksum(char* x, int bytes);

#endif
