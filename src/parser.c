#include "../include/parser.h"
#include <stdlib.h>
#include <stdio.h>

parser *initParser() {
  parser *p = malloc(sizeof(parser));
  return p;
}

void freeParser(parser *p) { free(p); }

AST *parse(parser *p, LList* tokens) {
  p->tokens = tokens;
  AST* root = initAST(T_IDENTIFIER, NULL);
  return root;
}

int accept(parser* p, tokenType type) {
  if (p->token->type == type) {
    nextToken(p);
    return 1;
  }
  return 0;
}

int expect(parser *p, tokenType type) {
  if (accept(p, type)) {
    return 1;
  }
  errorSyntax("Unexpected token", tokenTypeToStr(type));
  return 0;
}


token *nextToken(parser *p) {
  if (p->node->next != NULL) {
    p->node = p->node->next;
    p->token = p->node->data;
    return p->token;
  }
  return NULL;
}

AST* parse_id(parser* p) {
  expect(p, T_IDENTIFIER);
  token* t = p->token;
  if (accept(p, T_EQUALS)) {
    AST* ast = initAST(AST_ASSIGNMENT, t);
    return ast;
  }
  AST* ast = initAST(AST_VARIABLE, t);

  return ast;
}

AST* parse_function(parser* p) {
}

int acceptLit(parser* p) {
  switch (p->token->type) {
    default:
      return 0;
  }
}

AST* parse_term(parser *p) {
  AST* term = NULL;
  token* t = p->token;
  if (accept(p, T_IDENTIFIER)) {
    term = initAST(AST_ID, t);
  } else if (accept(p, T_INT)) {
    term = initAST(NODE_INTEGER, t);
  } else if (accept(p, T_PAREN_L)) {
    term = parse_expr(p);
    expect(p, T_PAREN_R);
  } else {
    errorSyntax("Unknown term", NULL);
  }
  return term;
}

int acceptOp(parser* p) {
  switch(p->token->type) {
    case T_OPERATOR:
      return accept(p, T_OPERATOR);
    case T_PLUS:
      return accept(p, T_PLUS);
    case T_MINUS:
      return accept(p, T_MINUS);
    case T_ASTERISK:
      return accept(p, T_ASTERISK);
    case T_SLASH:
      return accept(p, T_SLASH);
    default:
      return 0;
  }
}

AST* parse_expr(parser* p) {
  AST* term = parse_term(p);
  token* t = p->token;
  while (acceptOp(p)) {
    AST* op_node = initAST(AST_OPERATOR, t);
    op_node->l = term;
    op_node->r = parse_term(p);
    term = op_node;
  }
  return term;
}


void errorSyntax(const char *msg, const char* type) {
  printf("[Parser] %s : %s", msg, type);
  exit(1);
}


