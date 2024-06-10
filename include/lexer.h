#ifndef LEXER_H
#define LEXER_H

#include "../include/LList.h"
#include <stdbool.h>

#define COMMENT_CHAR '#'

typedef enum {
  T_COLON,
  T_SEMICOLON,
  T_PAREN_L,
  T_PAREN_R,
  T_BRACE_L,
  T_BRACE_R,
  T_DOT,
  T_COMMA,

  // Operators
  T_EQUALS,
  T_PLUS,
  T_MINUS,
  T_ASTERISK,
  T_SLASH,

  // Literals
  T_IDENTIFIER,
  T_INT_LIT,
  T_CHAR_LIT,
  T_BOOL_LIT,
  T_STR_LIT,
  T_FLOAT_LIT,

  // Keywords
  T_INT,
  T_F,
  T_CHAR,
  T_BOOL,
  T_STR,
  T_FLOAT,
  T_VOID,

  T_EOF

} TokenType;

typedef struct {
  int line;
  int column;
} CLoc;

typedef struct {
  TokenType type;
  char *value;
  CLoc loc;
} Token;

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

Token *getToken(Lexer *lexer);
Token *makeToken(const TokenType type, const char *value, const CLoc codeLoc);
void addToken(Lexer *lexer, Token *token);
bool atEnd(Lexer *lexer);
char peek(Lexer *lexer);
char next(Lexer *lexer);

void printToken(Token* token);

#endif
