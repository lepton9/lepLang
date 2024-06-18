#include "../include/dlist.h"

dlist *init_list(size_t item_size) {
  dlist *list = malloc(sizeof(dlist));
  list->size = 0;
  list->item_size = item_size;
  list->items = 0;
  return list;
}

void free_list(dlist* list) {
  if (list->items) {
    free(list->items);
  }
  free(list);
}

void list_add(dlist *list, void *item) {
  list->size++;
  if (!list->items) {
    list->items = malloc(list->item_size);
  } else {
    list->items = realloc(list->items, list->size * list->item_size);
  }
  list->items[list->size] = item;
}
