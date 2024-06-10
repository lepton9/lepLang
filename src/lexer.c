#include "../include/lexer.h"
#include <stdio.h>
#include <stdlib.h>

Lexer *initLexer() {
  Lexer *lexer = malloc(sizeof(Lexer));
  lexer->tokens = create_list();
  lexer->srcLen = 0;
  lexer->srcPos = 0;
  lexer->codeLoc.line = 1;
  lexer->codeLoc.column = 1;
  return lexer;
}

void freeLexer(Lexer *lexer) {
  list_clear(lexer->tokens);
  free(lexer->tokens);
  free(lexer);
}

void lex(Lexer *lexer) {
  while (!atEnd(lexer)) {
    Token *token = getToken(lexer);
  }

  Token *eof = makeToken(T_EOF, NULL, lexer->codeLoc);
  addToken(lexer, eof);
}

Token *getToken(Lexer *lexer) {
  char c = next(lexer);
  return NULL;
}

Token *makeToken(const TokenType type, const char *value, const CLoc codeLoc) {
  Token *token = malloc(sizeof(Token));
  *token = (Token){T_EOF, NULL, codeLoc};
  return token;
}

void addToken(Lexer *lexer, Token *token) { add_to_end(lexer->tokens, token); }

bool atEnd(Lexer *lexer) { return lexer->srcPos >= lexer->srcLen; }

char peek(Lexer *lexer) {
  if (atEnd(lexer)) {
    return '\0';
  }
  return lexer->src[lexer->srcPos + 1];
}

char next(Lexer *lexer) {
  lexer->codeLoc.column++;
  return lexer->src[lexer->srcPos++];
}

void nextLine(Lexer *lexer) {
  lexer->codeLoc.line++;
  lexer->codeLoc.column = 1;
}

void printToken(Token *token) {
  printf("Type: %d, Value: %s, Line: %d, Column: %d\n", token->type,
         token->value, token->loc.line, token->loc.column);
}
