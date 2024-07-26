#include "../include/analyzer.h"
#include <assert.h>
#include <stdio.h>

symtabStack* init_st_stack() {
  symtabStack* sts = malloc(sizeof(symtabStack));
  // sts->root = root;
  sts->cur_scope = 0;
  // sts->s = init_stack();
  sts->s = create_list();
  symtab* global_st = initSymbolTable();
  // push(sts->s, global_st);Q
  add_to_begin(sts->s, global_st);
  return sts;
}

void free_st_stack(symtabStack* sts) {
  while(!is_empty(sts->s)) {
    symtab* st = pop_front(sts->s);
    freeSymbolTable(st);
  }
  free(sts->s);
  free(sts);
}

void enter_scope(symtabStack* sts) {
  symtab* st = initSymbolTable();
  add_to_begin(sts->s, st);
  sts->cur_scope++;
}

void exit_scope(symtabStack* sts) {
  // if (sts->cur_scope == 0) return;
  assert(sts->cur_scope > 0);
  symtab* st = pop_front(sts->s);
  freeSymbolTable(st);
  sts->cur_scope--;
}

stEntry* lookup_all(symtabStack* sts, const char* key) {
  node* scope = sts->s->head;
  stEntry* e;
  while(scope != NULL) {
    e = lookup_scope(scope->data, key);
    if (e) return e;
    scope = scope->next;
  }
  return NULL;
}

stEntry* lookup_scope(symtab* st, const char* key) {
  return st_lookup(st, key);
}

symtab* currentScope(symtabStack* sts) {
  return sts->s->head->data;
}

stEntry* newVariable(symtabStack* sts, const char* id, const TYPE type) {
  stEntry* var = st_insert(currentScope(sts), id);
  var->type = type;
  return var;
}

// To be deleted
void addVar(symtabStack* sts, const char* id) {
  st_insert(currentScope(sts), id);
}

void semanticAnalysis(symtabStack* sts, AST* ast) {
  if (ast == NULL) return;

  switch(ast->type) {
    case AST_FUNCTION: {
      AST* header = ast->l;
      AST* body = ast->r;
      AST* param = header->l->next->l;
      token* name = header->l->value;
      if (lookup_all(sts, name->value)) {
        printf("Function %s already defined\n", name->value);
        exit(1);
      }
      stEntry* f = newVariable(sts, name->value, F);
      f->declLine = name->loc.line;
      f->f_info = malloc(sizeof(func_info));
      f->f_info->ret_type = ast->l->l->next->next->value->type;
      f->f_info->n_params = 0;
      // f_info->param_types = malloc(sizeof(int));
      while (param) {
        f->f_info->param_types = realloc(f->f_info->param_types, sizeof(int) * f->f_info->n_params + 1);
        f->f_info->param_types[f->f_info->n_params] = convertType(param->l->value->type);
        assert(param->type == AST_VARIABLE);
        f->f_info->n_params++;
        param = param->next;
      }
      enter_scope(sts);
      {
        semanticAnalysis(sts, ast->l);
        semanticAnalysis(sts, ast->r);
        printf("Function scope variables %d\n", (int)currentScope(sts)->n);
      }
      exit_scope(sts);
      semanticAnalysis(sts, ast->next);
      return;
    }
    case AST_VARIABLE: {
      AST* id = ast->r;
      if (id->type == AST_ASSIGNMENT) id = ast->r->l;
      if (lookup_all(sts, id->value->value)) {
        printf("Variable %s already defined\n", id->value->value);
        exit(1);
      }
      stEntry* var = newVariable(sts, id->value->value, convertType(ast->l->value->type));
      var->declLine = id->value->loc.line;
      break;
    }
    case AST_ASSIGNMENT: {
      bool valid = typecheck_assignment(sts, ast->l, ast->r);
      if (!valid) {
        printf("Assignment not valid\n");
        exit(1);
      }
      semanticAnalysis(sts, ast->next);
      return;
    }
    // case AST_OPERATOR: {
    //   AST* lhs = ast->l;
    //   AST* rhs = ast->r;
    //   int valid = typecheck_operator(sts, lhs, rhs);
    //   assert(valid >= 0);
    //   semanticAnalysis(sts, ast->next);
    //   return;
    // }
    default:
      break;
  }
  semanticAnalysis(sts, ast->l);
  semanticAnalysis(sts, ast->r);
  semanticAnalysis(sts, ast->next);
}

int funcRetType(AST* f) {
  return f->l->l->next->next->value->type;
}

TYPE convertType(const int type) {
  switch (type) {
    case T_KW_INT:
    case T_LIT_INT:
    case INT:
      return INT;
    case T_KW_FLOAT:
    case T_LIT_FLOAT:
    case FLOAT:
      return FLOAT;
    case T_KW_CHAR:
    case T_LIT_CHAR:
    case CHAR:
      return CHAR;
    case T_KW_STR:
    case T_LIT_STR:
    case STR:
      return CHAR;
    case T_KW_BOOL:
    case T_LIT_BOOL:
    case BOOL:
      return BOOL;
    case T_KW_MAIN:
    case T_KW_F:
    case F:
      return F;
    case T_KW_VOID:
    case VOID:
      return VOID;
    default:
      return -1;
  }
}

bool matchType(const int a, const int b) {
  return convertType(a) == convertType(b);
}

bool typecheck_fcall(symtabStack* sts, AST* fcall) {
  return 0;
}

bool typecheck_assignment(symtabStack* sts, AST* lhs, AST* rhs) {
  int lhs_type, rhs_type;
  char* id = lhs->value->value;
  stEntry* var = lookup_all(sts, id);
  if (!var) {
    printf("Variable %s not declared before assigning value\n", id);
    exit(1);
  }
  lhs_type = var->type;

  if (rhs->type == AST_OPERATOR) {
    rhs_type = typecheck_operator(sts, rhs->l, rhs->r);
  }
  else if (rhs->type == AST_FCALL) {
    char* f_id = rhs->l->value->value;
    stEntry* f = lookup_all(sts, f_id);
    if (!f) {
      printf("Function %s not declared before use\n", f_id);
      exit(1);
    }
    rhs_type = f->f_info->ret_type;
  }
  else if (rhs->type == AST_ID) {
    stEntry* var = lookup_all(sts, rhs->value->value);
    if (!var) {
      printf("Variable %s not declared before use\n", rhs->value->value);
      exit(1);
    }
    rhs_type = var->type;
  } else {
    rhs_type = rhs->value->type;
  }

  return matchType(lhs_type, rhs_type);
}

int typecheck_operator(symtabStack* sts, AST* lhs, AST* rhs) {
  int lt;
  int rt;
  if (lhs->type == AST_OPERATOR) lt = typecheck_operator(sts, lhs->l, lhs->r);
  else if (lhs->type == AST_FCALL) {
    char* id = lhs->l->value->value;
    stEntry* f = lookup_all(sts, id);
    if (!f) {
      printf("Function %s not declared before use\n", id);
      exit(1);
    }
    lt = f->f_info->ret_type;
  }
  else if (lhs->type == AST_ID) {
    stEntry* var = lookup_all(sts, lhs->value->value);
    if (!var) {
      printf("Variable %s not declared before use\n", lhs->value->value);
      exit(1);
    } else {
      lt = var->type;
    }
  }
  else lt = lhs->value->type;

  if (rhs->type == AST_OPERATOR) rt = typecheck_operator(sts, rhs->l, rhs->r);
  // if (rhs->type == AST_FCALL) rt = funcRetType(rhs); // Lookup st for function ret
  else if (rhs->type == AST_FCALL) {
    char* id = rhs->l->value->value;
    stEntry* f = lookup_all(sts, id);
    if (!f) {
      printf("Function %s not declared before use\n", id);
      exit(1);
    }
    rt = f->f_info->ret_type;
  }
  else if (rhs->type == AST_ID) {
    stEntry* var = lookup_all(sts, rhs->value->value);
    if (!var) {
      printf("Variable %s not declared before use\n", rhs->value->value);
      exit(1);
    } else {
      rt = var->type;
    }
  }
  else rt = rhs->value->type;

  if (matchType(lt, rt)) return lt;
  else return -1;
}

void checkAST(AST* root) {
  symtabStack* st_stack = init_st_stack();

  semanticAnalysis(st_stack, root);

  printf("Global variables %d\n", (int)currentScope(st_stack)->n);
  free_st_stack(st_stack);

}

