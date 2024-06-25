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
int acceptType(parser* p);
int accept(parser *p, tokenType type);
int expect(parser *p, tokenType type);
token *nextToken(parser *p);
void errorSyntax(const char *msg, const token* t, const char* expected);
// token *peek(parser *p);


AST* parse_func_def(parser* p);
AST* parse_func_header(parser* p);
AST* parse_func_body(parser* p);
AST* parse_func_params(parser* p);
AST* parse_func_param(parser* p);


AST* parse_assignment(parser* p);
AST* parse_var_decl(parser* p);

AST* parse_type(parser* p);
AST* parse_id(parser* p);
AST* parse_statements(parser *p);
AST* parse_statement(parser *p);
AST* parse_term(parser *p);
AST* parse_expr(parser *p);
AST* parse_return(parser *p);

#endif
