#ifndef ANALYZER_H
#define ANALYZER_H

#include "../include/ast.h"
#include "../include/symtab.h"
#include "../include/stack.h"

typedef enum {
  INT = 200,
  FLOAT,
  CHAR,
  STR,
  BOOL,
  F,
  VOID
} TYPE;

typedef struct {
  int cur_scope;
  // stack* s;
  LList* s;
  // AST* root;

} symtabStack;

symtabStack* init_st_stack();
void free_st_stack(symtabStack* sts);

symtab* currentScope(symtabStack* sts);
void enter_scope(symtabStack* sts);
void exit_scope(symtabStack* sts);

bool matchType(const int a, const int b);
int funcRetType(AST* f);
TYPE convertType(const int type);

void addVar(symtabStack* sts, const char* id);
stEntry* newVariable(symtabStack* sts, const char* id, const TYPE type);

stEntry* lookup_all(symtabStack* sts, const char* key);
stEntry* lookup_scope(symtab* st, const char* key);

void checkAST(AST* root);
void semanticAnalysis(symtabStack* sts, AST* ast);

bool typecheck_assignment(symtabStack* sts, AST* lhs, AST* rhs);
int typecheck_operator(symtabStack* sts, AST* lhs, AST* rhs);
bool typecheck_expr(symtabStack* sts, AST* ast);


#endif
