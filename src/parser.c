#include "../include/parser.h"
#include <stdlib.h>

parser *initParser() {
  parser *p = malloc(sizeof(parser));
  return p;
}

void freeParser(parser *p) { free(p); }

AST *parse(parser *p, LList* tokens) {
  p->tokens = tokens;
  return NULL;
}

void match(tokenType type) {
  // if (current token.type == type
}

token *nextToken(parser *p) {}
