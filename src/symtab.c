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
  if (entry->f_info) {
    free(entry->f_info->param_types);
    free(entry->f_info);
  }
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

void print_symtab(FILE* out, const symtab* st) {
  fprintf(out, "\n<-------------------->\n");
  for (int i = 0; i < st->size; i++) {
    if (st->items[i]) print_stEntry(out, st->items[i]->value);
  }
  fprintf(out, ">--------------------<\n");
}

void print_stEntry(FILE* out, const stEntry* e) {
  fprintf(out, "%d:%s decl: %d", e->type, e->name, e->declLine);
  if (e->f_info) {
    fprintf(out, " ret: %d args: ( ", e->f_info->ret_type);
    for (int i = 0; i < e->f_info->n_params; i++) {
      fprintf(out, "%d ", e->f_info->param_types[i]);
    }
    fprintf(out, ")");
  }
  fprintf(out, "\n");
}

