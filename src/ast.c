#include "../include/ast.h"


AST* initAST(int type, token* t) {
  AST* ast = malloc(sizeof(AST));
  ast->type = type;
  // ast->value = malloc(sizeof(token));
  ast->value = t;
  ast->l = NULL;
  ast->r = NULL;
  // ast->children = init_list(sizeof(AST));;
  return ast;
}

void freeAST(AST *ast) {
  // free_list(ast->children);
  // free(ast->value);
  free(ast);
}


