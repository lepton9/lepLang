#include "../include/ast.h"


AST* initAST(int type) {
  AST* ast = malloc(sizeof(AST));
  ast->type = type;

  ast->children = init_list(sizeof(AST));;

  return ast;
}

void freeAST(AST *ast) {
  free_list(ast->children);
  free(ast);
}


