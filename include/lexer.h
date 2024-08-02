#ifndef LEXER_H
#define LEXER_H

#include "../include/LList.h"
#include "../include/token.h"
#include <stdbool.h>
#include <string.h>

#define COMMENT_CHAR '#'

typedef struct {
  char* msg;
  int beg;
  int len;
  cLoc codeLoc;
} lexError;

typedef struct {
  LList *tokens;
  LList *errors;
  char *src;
  int srcLen;
  int srcPos;
  cLoc codeLoc;
} Lexer;

Lexer *initLexer();
void freeLexer(Lexer *lexer);

LList* lex(Lexer *lexer); // Tokenize

token *getNextToken(Lexer *lexer);
void addToken(Lexer *lexer, token *token);
bool atEnd(Lexer *lexer);
char peek(Lexer *lexer);
char advance(Lexer *lexer);
void nextLine(Lexer *lexer);
tokenType isKeyword(Lexer *lexer, const char *value);

token* makeTokenN(Lexer *lexer, const tokenType type, const int beg, const cLoc cl);
lexError* lexerError(Lexer *lexer, const char* msg, const int beg, const int len);
void addSynError(Lexer *lexer, lexError * err);

void printTokens(Lexer *lexer);
void printErrors(Lexer *lexer);
void printError(Lexer *lexer, lexError *err);

char* malStrncpy(const char *s, const size_t n);

char* getLine(const char* src, const size_t len, const int line);

#endif
