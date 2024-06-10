#ifndef LLIST_H
#define LLIST_H

typedef struct node {
  void *data;
  struct node *next;
} node;

typedef struct LList {
  int size;
  node *head;
} LList;

LList *create_list();
int get_size(LList *list);
node *copy_nodes(node *head);
void copy_list(LList *listTo, LList *listFrom);
void add_to_begin(LList *list, void *data);
void add_to_end(LList *list, void *data);
void *pop_front(LList *list);
void *pop_last(LList *list);
int is_empty(LList *list);
int list_size(LList *list);
void list_clear(LList *list);

#endif
