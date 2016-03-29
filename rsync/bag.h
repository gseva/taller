#ifndef __BAG_H__
#define __BAG_H__

/**
 * La bolsa (bag) es una estructura sencilla que permite guardar items de tipo
 * int, y luego buscarlos por valor. Lo justo para guardar checksums en el
 * servidor.
 *
 * Está implementada como una lista enlazada.
 */

typedef struct bag_node_t {
  int item;
  struct bag_node_t* next;
  struct bag_node_t* previous;
} bag_node_t;

typedef struct {
  bag_node_t* initial;
  bag_node_t* current;
} bag_t;


/**
 * Crea de la bolsa.
 */
int bag_create(bag_t* b);

/**
 * Agrega un elemento en la bolsa en la posición actual.
 */
int bag_add(bag_t* b, int item);

/**
 * Busca el elemento y devuelve su índice en la bolsa o -1 en el caso de
 * que no se encuentre.
 */
int bag_search(bag_t* b, int item);

/**
 * Libera los recursos tomados por la creación.
 */
int bag_destroy(bag_t* b);

#endif
