#ifndef AST_H
#define AST_H

#include "../include/dlist.h"
#include "../include/token.h"

typedef struct AST_NODE {
  enum {
    AST_STATEMENT,
    AST_VARIABLE,
    AST_FUNCTION,
    AST_ASSIGNMENT,
    AST_EXPR,
    AST_ID,
    AST_OPERATOR,
  } type;

  token* value;

  struct AST_NODE* l;
  struct AST_NODE* r;
  // dlist *children;
} AST;

AST *initAST(int type, token* t);
void freeAST(AST *ast);

#endif
