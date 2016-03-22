#ifndef __BAG_H__
#define __BAG_H__

typedef struct bag_node_t {
  int item;
  struct bag_node_t* next;
  struct bag_node_t* previous;
} bag_node_t;

typedef struct {
  bag_node_t* initial;
  bag_node_t* current;
} bag_t;

int bag_create(bag_t* b);

int bag_add(bag_t* b, int item);

int bag_search(bag_t* b, int item);

int bag_destroy(bag_t* b);

#endif
