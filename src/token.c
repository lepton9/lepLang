#include "../include/token.h"
#include <stdlib.h>
#include <stdio.h>

token *makeToken(const tokenType type, const char *value, const CLoc codeLoc) {
  token *tok = malloc(sizeof(token));
  *tok = (token){type, (char*)value, codeLoc};
  return tok;
}

const char* tokenTypeToStr(tokenType type) {
  switch (type) {
    case T_ARROW:
      return "T_ARROW";
    case T_SPACE:
      return "T_SPACE";
    case T_NEWLINE:
      return "T_NEWLINE";
    case T_COLON:
      return "T_COLON";
    case T_SEMICOLON:
      return "T_SEMICOLON";
    case T_PAREN_L:
      return "T_PAREN_L";
    case T_PAREN_R:
      return "T_PAREN_R";
    case T_BRACE_L:
      return "T_BRACE_L";
    case T_BRACE_R:
      return "T_BRACE_R";
    case T_DOT:
      return "T_DOT";
    case T_COMMA:
      return "T_COMMA";
    case T_EQUALS:
      return "T_EQUALS";
    case T_PLUS:
      return "T_PLUS";
    case T_MINUS:
      return "T_MINUS";
    case T_ASTERISK:
      return "T_ASTERISK";
    case T_SLASH:
      return "T_SLASH";
    case T_IDENTIFIER:
      return "T_IDENTIFIER";
    case T_INT_LIT:
      return "T_INT_LIT";
    case T_CHAR_LIT:
      return "T_CHAR_LIT";
    case T_BOOL_LIT:
      return "T_BOOL_LIT";
    case T_STR_LIT:
      return "T_STR_LIT";
    case T_FLOAT_LIT:
      return "T_FLOAT_LIT";
    case T_INT:
      return "T_INT";
    case T_F:
      return "T_F";
    case T_CHAR:
      return "T_CHAR";
    case T_BOOL:
      return "T_BOOL";
    case T_STR:
      return "T_STR";
    case T_FLOAT:
      return "T_FLOAT";
    case T_VOID:
      return "T_VOID";
    case T_RET:
      return "T_RET";
    case T_ERROR:
      return "T_ERROR";
    case T_EOF:
      return "T_EOF";
  }
  return "UNDEFINED";
}


void printToken(token *tok) {
  printf("%-13s %-10s L%-2d C%d\n", tokenTypeToStr(tok->type), tok->value, tok->loc.line, tok->loc.column);
}

