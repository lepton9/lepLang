#ifndef SYMTAB_H
#define SYMTAB_H

#include "../include/ast.h"
#include "../include/hashtab.h"
#include "../include/LList.h"
#include <stdio.h>

typedef struct {
  int ret_type;
  size_t n_params;
  int* param_types;
} func_info;

typedef struct {
  char* name;
  int type;
  char* scope;
  int declLine;
  LList* usageLines;
  size_t size;
  int address;
  func_info* f_info;
  AST* node;
} stEntry;

typedef hashtab symtab;

// typedef struct {
//   hashtab* table;
//   char* scope;
// } symtab;

symtab* initSymbolTable();
void freeSymbolTable(symtab* st);
void free_stEntry(stEntry* entry);

stEntry* st_insert(symtab* st, const char* key);
stEntry* st_lookup(symtab* st, const char* identifier);

void print_symtab(FILE* out, const symtab* st);
void print_stEntry(FILE* out, const stEntry* e);

// void hide(symtab* table, char** scope);
//
// void set(symtab* table); // Beginning of block
// void reset(symtab* table); // End of block

#endif
