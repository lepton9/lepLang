#ifndef PARSER_H
#define PARSER_H

#include "../include/LList.h"
// #include "../include/lexer.h"
#include "../include/ast.h"
#include "../include/token.h"

typedef struct {
  LList *tokens;
  // Lexer *lexer;

  node *node;
  token *token;
} parser;

parser *initParser();
void freeParser(parser *p);
AST *parse(parser *p, LList *tokens);
// void match(tokenType type);
int acceptOp(parser* p);
int accept(parser *p, tokenType type);
int expect(parser *p, tokenType type);
token *nextToken(parser *p);
void errorSyntax(const char *msg, const char* type);
// token *peek(parser *p);


AST* parse_term(parser *p);
AST* parse_expr(parser *p);

#endif
