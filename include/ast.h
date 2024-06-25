#ifndef AST_H
#define AST_H

#include "../include/dlist.h"
#include "../include/token.h"

typedef struct AST_NODE {
  enum {
    AST_STATEMENT,
    AST_VARIABLE,
    AST_TYPE,
    AST_FUNCTION,
    AST_PARAMETER,
    AST_ASSIGNMENT,
    AST_EXPR,
    AST_ID,
    AST_VALUE,
    AST_OPERATOR,
    AST_RET,
  } type;

  token* value;

  struct AST_NODE* l;
  struct AST_NODE* r;
  // dlist *children;
} AST;

AST *initAST(int type, token* t);
void freeAST(AST *ast);
void printAST(AST* ast, int indent);
void print_ast(AST* ast, int indent);

#endif
