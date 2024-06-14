#ifndef LEXER_H
#define LEXER_H

#include "../include/LList.h"
#include "../include/token.h"
#include <stdbool.h>

#define COMMENT_CHAR '#'

typedef struct {
  LList *tokens;
  char *src;
  int srcLen;
  int srcPos;
  CLoc codeLoc;
} Lexer;

Lexer *initLexer();
void freeLexer(Lexer *lexer);

void lex(Lexer *lexer); // Tokenize

token *getNextToken(Lexer *lexer); // getNextToken()
void addToken(Lexer *lexer, token *token);
bool atEnd(Lexer *lexer);
char peek(Lexer *lexer);
char next(Lexer *lexer); // advance()
void nextLine(Lexer *lexer);
tokenType isKeyword(Lexer *lexer, const char *value);

token* makeTokenN(Lexer* lexer, const tokenType type, const int beg, const CLoc cl);

#endif
