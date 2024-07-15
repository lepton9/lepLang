#ifndef SYMTAB_H
#define SYMTAB_H

#include "../include/ast.h"

typedef struct {
  // char** name;
  int type;
  char** scope;
  AST* node;

} stEntry;

typedef struct {

} symtab;

symtab* initSymbolTable();


void insert(symtab* table);
void lookup(symtab* table, char** identifier);
void hide(symtab* table, char** scope);

#endif
