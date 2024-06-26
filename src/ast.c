#include "../include/ast.h"
#include <stdio.h>

AST *initAST(int type, token *t) {
  AST *ast = malloc(sizeof(AST));
  ast->type = type;
  // ast->value = malloc(sizeof(token));
  ast->value = t;
  ast->l = NULL;
  ast->r = NULL;
  // ast->children = init_list(sizeof(AST));;
  return ast;
}

void freeAST(AST *ast) {
  if (ast) {
    freeAST(ast->l);
    freeAST(ast->r);
    free(ast);
  }
}

void printAST(AST *node, int indent) {
  if (!node)
    return;

  for (int i = 0; i < indent; i++)
    printf("  ");

  switch (node->type) {
    case AST_FUNCTION:
      printf("Function\n");
      break;
    case AST_STATEMENT:
      printf("Statement\n");
      break;
    case AST_PARAMETER:
      printf("Parameter\n");
      break;
    case AST_VARIABLE:
      printf("Variable\n");
      break;
    case AST_ASSIGNMENT:
      printf("Assignment\n");
      break;
    case AST_RET:
      printf("Return\n");
      break;
    case AST_EXPR:
      printf("Expression\n");
      break;
    case AST_TYPE:
      printf("Type: %s\n", (char*)node->value->value);
      break;
    case AST_ID:
      printf("Identifier: %s\n", node->value->value);
      break;
    case AST_OPERATOR:
      printf("Operator: %s\n", node->value->value);
      break;
    case AST_VALUE:
      printf("Value: %s\n", node->value->value);
      break;
    default:
      printf("Undefined\n");
  }

  printAST(node->l, indent + 1);
  printAST(node->r, indent + 1);
}

void print_ast(AST* node, int indent) {
  if (!node)
    return;
  for (int i = 0; i < indent; i++)
    printf("  ");

  if (node->value) {
    printf("%s\n", node->value->value);
  } else {
    printf("Union\n");
  }

  print_ast(node->l, indent + 1);
  print_ast(node->r, indent + 1);
}

