#ifndef AST_H
#define AST_H

#include "../include/dlist.h"

typedef struct {
  enum {
    AST_STATEMENT,
    AST_VARIABLE,
    AST_FUNCTION_DEF,

  } type;

  dlist *children;
} AST;

AST *initAST(int type);
void freeAST(AST *ast);

#endif
