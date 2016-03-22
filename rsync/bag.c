
#include <stddef.h>

#include "bag.h"


int bag_create(bag_t* b) {
  bag_node_t initial;
  initial.next = NULL;
  b->current = b->initial = &initial;
  return 0;
}

int bag_add(bag_t* b, int item) {
  b->current->item = item;
  bag_node_t node;
  node.next = NULL;
  node.previous = b->current;
  b->current->next = &node;
  b->current = &node;
  return 0;
}

int bag_search(bag_t* b, int item) {
  b->current = b->initial;
  if (b->current == NULL) return -1;
  int i = 0;
  while (b->current->next != NULL) {
    if (b->current->item == item) return i;
    b->current = b->current->next;
    i++;
  }
  return -1;
}

int bag_remove(bag_t* b) {
  if (b->current == b->initial) {
    b->initial = b->current = NULL;
  }
  else if (b->current->next) {
    b->current->next->previous = b->current->previous;
    b->current = b->current->next;
  } else {
    b->current = b->current->previous;
    b->current->next = NULL;
  }
  return 0;
}

int bag_destroy(bag_t* b) {
  return 0;
}
