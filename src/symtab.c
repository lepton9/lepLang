#include "../include/symtab.h"
#include <string.h>


symtab* initSymbolTable() {
  symtab* st = init_hashtabn(41);
  return st;
}

void freeSymbolTable(symtab* st) {
  for (int i = 0; i < st->size; i++) {
    if (st->items[i]) {
      free_stEntry(st->items[i]->value);
      free(st->items[i]);
    }
  }
  free(st->items);
  free(st);
}

void free_stEntry(stEntry* entry) {
  free(entry->name);
  free(entry->usageLines);
  free(entry);
}

stEntry* st_insert(symtab* st, const char* key) {
  stEntry* e = malloc(sizeof(stEntry));
  e->name = malloc(strlen(key) + 1);
  strcpy(e->name, key);
  e->usageLines = create_list();
  // e->name = (char*)key;
  ht_e* hte = ht_insert(st, key, e);
  return hte->value;
}

stEntry* st_lookup(symtab* st, const char* identifier) {
  stEntry* e = (stEntry*)ht_get(st, identifier);
  return e;
}

