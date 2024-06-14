#ifndef TOKEN_H
#define TOKEN_H

#define COMMENT_CHAR '#'

typedef enum {
  T_ARROW,
  T_COLON,
  T_SEMICOLON,
  T_PAREN_L,
  T_PAREN_R,
  T_BRACE_L,
  T_BRACE_R,
  T_DOT,
  T_COMMA,
  T_SPACE,
  T_NEWLINE,

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
  T_RET,

  T_ERROR,

  T_EOF

} tokenType;

// char keywords[20] = {"int", "float", "char", "bool", "str", "void", "f", "while", "ret", "main", "if", "else"};

typedef struct {
  int line;
  int column;
} CLoc;

typedef struct {
  tokenType type;
  char *value;
  CLoc loc;
} token;

// Maybe better
// typedef struct {
//   tokenType type;
//   int lenght;
//   CLoc loc; // start of the token
// } Token;

token *makeToken(const tokenType type, const char *value, const CLoc codeLoc);
const char* tokenTypeToStr(tokenType type);
void printToken(token* token);

#endif
