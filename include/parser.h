#ifndef PARSER_H
#define PARSER_H

#include "../include/LList.h"
// #include "../include/lexer.h"
#include "../include/ast.h"
#include "../include/token.h"

typedef struct {
  LList *tokens;
  // Lexer *lexer;

  token *tokenCur;
} parser;

parser *initParser();
void freeParser(parser *p);
AST *parse(parser *p, LList *tokens);
void match(tokenType type);
token *nextToken(parser *p);
// token *peek(parser *p);

#endif
