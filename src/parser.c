#include "../include/parser.h"
#include <stdlib.h>
#include <stdio.h>

parser *initParser(Lexer* lexer) {
  parser *p = malloc(sizeof(parser));
  p->lexer = lexer;
  return p;
}

void freeParser(parser *p) { free(p); }

AST *parse(parser *p) {
  p->node = p->lexer->tokens->head;
  p->token = p->lexer->tokens->head->data;

  AST* root = parse_func_def(p);
  return root;
}

int accept(parser* p, tokenType type) {
  if (p->token->type == type) {
    nextToken(p);
    return 1;
  }
  return 0;
}

int expect(parser *p, tokenType type) {
  if (accept(p, type)) {
    return 1;
  }
  errorSyntax(p, "Unexpected token", tokenTypeToStr(type));
  return 0;
}

int acceptOp(parser* p) {
  switch(p->token->type) {
    case T_OPERATOR:
      return accept(p, T_OPERATOR);
    case T_PLUS:
      return accept(p, T_PLUS);
    case T_MINUS:
      return accept(p, T_MINUS);
    case T_ASTERISK:
      return accept(p, T_ASTERISK);
    case T_SLASH:
      return accept(p, T_SLASH);
    default:
      return 0;
  }
}

int acceptType(parser* p) {
  if (isType(p)) {
    return accept(p, p->token->type);
  } else {
    return 0;
  }
}

int isType(parser* p) {
  switch(p->token->type) {
    case T_KW_INT:
    case T_KW_F:
    case T_KW_CHAR:
    case T_KW_BOOL:
    case T_KW_STR:
    case T_KW_FLOAT:
    case T_KW_VOID:
      return 1;
    default:
      return 0;
  }
}

int acceptLit(parser* p) {
  switch (p->token->type) {
    default:
      return 0;
  }
}

token *nextToken(parser *p) {
  if (p->node->next != NULL) {
    p->node = p->node->next;
    p->token = p->node->data;
    return p->token;
  }
  return NULL;
}

AST* parse_id(parser* p) {
  token* t = p->token;
  expect(p, T_IDENTIFIER);
  AST* ast = initAST(AST_ID, t);
  return ast;
}

AST* parse_type(parser* p) {
  AST* type = initAST(AST_TYPE, p->token);
  acceptType(p);
  return type;
}

AST* parse_func_def(parser* p) {
  token* t = p->token;
  expect(p, T_KW_F);
  expect(p, T_COLON);
  AST* header = parse_func_header(p);
  AST* body = parse_func_body(p);
  AST* func = initAST(AST_FUNCTION, t);
  func->l = header;
  func->r = body;
  return func;
}

AST* parse_func_header(parser* p) {
  AST* name = parse_id(p);
  expect(p, T_PAREN_L);
  AST* params = parse_func_params(p);
  expect(p, T_PAREN_R);
  expect(p, T_ARROW);
  AST* ret_type = parse_type(p);

  AST* header = initAST(AST_FUNCTION, NULL);
  header->l= name;
  header->r= initAST(AST_PARAMETER, NULL);
  header->r->l= params;
  header->r->r= ret_type;
  return header;
}

AST* parse_func_body(parser* p) {
  expect(p, T_BRACE_L);
  AST* body = parse_statements(p);
  expect(p, T_BRACE_R);
  return body;
}

AST* parse_func_params(parser* p) {
  AST* param = parse_func_param(p);
  AST* param_list = param;
  while(accept(p, T_COMMA)) {
    AST* next_param = parse_func_param(p);
    AST* new_param_list = initAST(AST_PARAMETER, NULL);
    new_param_list->l = param_list;
    new_param_list->r = next_param;
    param_list = new_param_list;
  }
  return param_list;
}

AST* parse_func_param(parser* p) {
  AST* param = NULL;
  token* t = p->token;
  if (acceptType(p)) {
    expect(p, T_COLON);
    AST* type = initAST(AST_TYPE, t);
    AST* id = parse_id(p);
    param = initAST(AST_PARAMETER, NULL);
    param->l = type;
    param->r = id;
  }
  return param;
}

AST* parse_term(parser *p) {
  AST* term = NULL;
  token* t = p->token;
  if (accept(p, T_IDENTIFIER)) {
    term = initAST(AST_ID, t);

  } else if (accept(p, T_LIT_INT)) {
    term = initAST(AST_VALUE, t);

  } else if (accept(p, T_PAREN_L)) {
    term = parse_expr(p);
    expect(p, T_PAREN_R);
  } else {
    errorSyntax(p, "Unknown term", NULL);
  }
  return term;
}


AST* parse_statements(parser *p) {
  AST *statement = parse_statement(p);
  AST *statement_list = statement;
  while (p->token->type != T_BRACE_R) {
    AST *next_statement = parse_statement(p);
    AST *new_statement_list = initAST(AST_STATEMENT, NULL);
    new_statement_list->l = statement_list;
    new_statement_list->r = next_statement;
    statement_list = new_statement_list;
  }
  return statement_list;
}

AST* parse_var_decl(parser* p) {
  AST* type = initAST(AST_TYPE, p->token);
  acceptType(p);
  expect(p, T_COLON);
  AST* id = initAST(AST_ID, p->token);
  expect(p, T_IDENTIFIER);
  AST* var = initAST(AST_VARIABLE, NULL);
  var->l = type;
  var->r = id;
  if (p->token->type == T_EQUALS) {
    AST* assignment = initAST(AST_ASSIGNMENT, p->token);
    expect(p, T_EQUALS);
    AST* expr = parse_expr(p);
    assignment->l = id;
    assignment->r = expr;
    var->r = assignment;
  }
  return var;
}

AST* parse_assignment(parser* p) {
  AST* id = initAST(AST_ID, p->token);
  expect(p, T_IDENTIFIER);
  AST* assignment = initAST(AST_ASSIGNMENT, p->token);
  expect(p, T_EQUALS);
  AST* expr = parse_expr(p);
  assignment->l = id;
  assignment->r = expr;
  return assignment;
}

AST* parse_func_call(parser* p) {
  return NULL;
}

AST* parse_statement(parser *p) {
  AST *statement = NULL;
  token* t = p->token;
  if (isType(p)) {
    statement = parse_var_decl(p);
  } else if (p->token->type == T_IDENTIFIER) {
    statement = parse_assignment(p);
  } else if (p->token->type == T_KW_RET) {
    statement = parse_return(p);
  } else {
    errorSyntax(p, "Unknown statement", NULL);
    exit(1);
  }
  expect(p, T_SEMICOLON);
  return statement;
}

AST* parse_return(parser *p) {
  token* t = p->token;
  expect(p, T_KW_RET);
  AST* expr_node = parse_expr(p);
  AST* ret_node = initAST(AST_RET, t);
  ret_node->l = expr_node;
  return ret_node;
}

AST* parse_expr(parser* p) {
  AST* term = parse_term(p);
  token* t = p->token;
  while (acceptOp(p)) {
    AST* op_node = initAST(AST_OPERATOR, t);
    op_node->l = term;
    op_node->r = parse_term(p);
    term = op_node;
  }
  return term;
}

int digits(int n)
{
  int count = 0;
  if (n > 0) {
      count++;
      digits(n / 10);
  }
  return count;
}

void errorSyntax(parser* p, const char *msg, const char* expected) {
  if (expected) {
    printf("[Parser] %s : %s != %s, at L%-2d C%d\n", msg, tokenTypeToStr(p->token->type), expected, p->token->loc.line, p->token->loc.column);
  } else {
    printf("[Parser] %s : %s, at L%-2d C%d\n", msg, tokenTypeToStr(p->token->type), p->token->loc.line, p->token->loc.column);
  }
  printf("%d: %s\n", p->token->loc.line, getLine(p->lexer, p->token->loc.line));
  printf("%*c^\n", p->token->loc.column + digits(p->token->loc.line) + 1, ' ');
  exit(1);
}


