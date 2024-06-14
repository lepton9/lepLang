#include "../include/lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  token *tok;
  while (!atEnd(lexer)) {
    tok = getNextToken(lexer);
    if (tok) {
      addToken(lexer, tok);
    }
  }

  token *t_eof = makeToken(T_EOF, NULL, lexer->codeLoc);
  addToken(lexer, t_eof);
}

token *getNextToken(Lexer *lexer) {
  int begI = lexer->srcPos;
  CLoc cLocB = lexer->codeLoc;
  char* tv;
  char* c = malloc(sizeof(char));
  *c = next(lexer);
  token *token;
  switch (*c) {
    case ' ': // Or break
      // token = makeToken(T_SPACE, c, cLocB);
      return NULL;
      break;
    case '\n': // Or break
      // token = makeToken(T_NEWLINE, c, cLocB);
      nextLine(lexer);
      return NULL;
      break;
    case ':':
      token = makeToken(T_COLON, c, cLocB);
      break;
    case ';':
      token = makeToken(T_SEMICOLON, c, cLocB);
      break;
    case '(':
      token = makeToken(T_PAREN_L, c, cLocB);
      break;
    case ')':
      token = makeToken(T_PAREN_R, c, cLocB);
      break;
    case '{':
      token = makeToken(T_BRACE_L, c, cLocB);
      break;
    case '}':
      token = makeToken(T_BRACE_R, c, cLocB);
      break;
    case '.':
      token = makeToken(T_DOT, c, cLocB);
      break;
    case ',':
      token = makeToken(T_COMMA, c, cLocB);
      break;
    case '=':
      token = makeToken(T_EQUALS, c, cLocB);
      break;
    case '+':
      token = makeToken(T_PLUS, c, cLocB);
      break;
    case '-':
      if (peek(lexer) == '>') {
        next(lexer);
        return makeTokenN(lexer, T_ARROW, begI, cLocB);
      }
      token = makeToken(T_MINUS, c, cLocB);
      break;
    case '*':
      token = makeToken(T_ASTERISK, c, cLocB);
      break;
    case '/':
      token = makeToken(T_SLASH, c, cLocB);
      break;

    default:
      if (isalpha(*c)) {
        while (isalpha(peek(lexer))) {
          next(lexer);
        }
        tv = malloc((lexer->srcPos - begI) * sizeof(char));
        strncpy(tv, lexer->src + begI, lexer->srcPos - begI);
        tokenType tt = isKeyword(lexer, tv);
        // tt = (tt >= 0) ? tt : T_IDENTIFIER;
        token = makeToken(tt, tv, cLocB);
      }
      else if (isdigit(*c)) {
        tokenType tt = T_INT_LIT;
        while ((*c = peek(lexer)) && isdigit(*c)) {
          next(lexer);
        }
        if (peek(lexer) == '.') {
          next(lexer);
          if (isdigit(peek(lexer))) {
            tt = T_FLOAT_LIT;
            while ((*c = peek(lexer)) && isdigit(*c)) {
              next(lexer);
            }
          } else {
            // Make error function and the error msg as value 
            return makeToken(T_ERROR, c, cLocB);
          }
        }
        token = makeTokenN(lexer, tt, begI, cLocB);
      }
      else {
        token = makeToken(T_ERROR, c, cLocB);
      }
      break;
  }

  // If no match for token add error
  // token *token = makeToken(T_ERROR, value, lexer->codeLoc);
  return token;
}

tokenType isKeyword(Lexer *lexer, const char* value) {
  if (strcmp(value, "int") == 0) return T_INT;
  if (strcmp(value, "char") == 0) return T_CHAR;
  if (strcmp(value, "bool") == 0) return T_BOOL;
  if (strcmp(value, "str") == 0) return T_STR;
  if (strcmp(value, "float") == 0) return T_FLOAT;
  if (strcmp(value, "void") == 0) return T_VOID;
  if (strcmp(value, "f") == 0) return T_F;
  if (strcmp(value, "ret") == 0) return T_RET;
  return T_IDENTIFIER;
}

token* makeTokenN(Lexer* lexer, const tokenType type, const int beg, const CLoc cl) {
  char* tval = malloc((lexer->srcPos - beg) * sizeof(char));
  strncpy(tval, lexer->src + beg, lexer->srcPos - beg);
  return makeToken(type, tval, cl);
}

void addToken(Lexer *lexer, token *token) { add_to_end(lexer->tokens, token); }

bool atEnd(Lexer *lexer) { return lexer->srcPos >= lexer->srcLen; }

char peek(Lexer *lexer) {
  if (atEnd(lexer)) {
    return '\0';
  }
  return lexer->src[lexer->srcPos];
  // return lexer->src[lexer->srcPos + 1];
}

char next(Lexer *lexer) {
  lexer->codeLoc.column++;
  return lexer->src[lexer->srcPos++];
}

void nextLine(Lexer *lexer) {
  lexer->codeLoc.line++;
  lexer->codeLoc.column = 1;
}
