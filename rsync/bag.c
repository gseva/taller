
#include <stddef.h>
#include <stdlib.h>

#include "bag.h"


int bag_create(bag_t* b) {
  b->initial = malloc(sizeof(bag_node_t));
  b->current = b->initial;
  b->initial->next = NULL;
  return 0;
}

int bag_add(bag_t* b, int item) {
  b->current->item = item;
  b->current->next = malloc(sizeof(bag_node_t));
  b->current->next->previous = b->current;
  b->current = b->current->next;
  b->current->next = NULL;
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

int bag_destroy(bag_t* b) {
  b->current = b->initial;
  while (b->current->next) {
    bag_node_t* temp = b->current->next;
    free(b->current);
    b->current = temp;
  }
  free(b->current);
  return 0;
}
