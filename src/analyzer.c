#include "../include/analyzer.h"
#include <assert.h>
#include <stdio.h>

symtabStack* init_st_stack() {
  symtabStack* sts = malloc(sizeof(symtabStack));
  sts->cur_scope = 0;
  sts->s = create_list();
  sts->contexts = create_list();
  symtab* global_st = initSymbolTable();
  add_to_begin(sts->s, global_st);
  return sts;
}

void free_st_stack(symtabStack* sts) {
  while(!is_empty(sts->s)) {
    symtab* st = pop_front(sts->s);
    freeSymbolTable(st);
  }
  free(sts->s);
  free(sts->contexts);
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
      f->f_info->ret_type = convertType(ast->l->l->next->next->value->type);
      f->f_info->n_params = 0;
      while (param) {
        f->f_info->param_types = realloc(f->f_info->param_types, sizeof(int) * f->f_info->n_params + 1);
        f->f_info->param_types[f->f_info->n_params] = convertType(param->l->value->type);
        assert(param->type == AST_VARIABLE);
        f->f_info->n_params++;
        param = param->next;
      }
      enter_scope(sts);
      {
        semanticAnalysis(sts, header);

        context* c = malloc(sizeof(context));
        c->ret_type = f->f_info->ret_type;
        c->func_name = f->name;
        c->returned = false;
        c->ret_scope = -1;
        add_to_begin(sts->contexts, c);

        semanticAnalysis(sts, body);

        if (f->f_info->ret_type != VOID) {
          if (!c->returned) {
            printf("Non void function %s not returning a value\n", f->name);
            exit(1);
          }
          if (c->ret_scope != sts->cur_scope) {
            printf("Function %s not returning from all code paths\n", f->name);
            exit(1);
          }
        }

        pop_front(sts->contexts);
        free(c);
        // printf("Function scope variables %d\n", (int)currentScope(sts)->n);
        print_symtab(stdout, currentScope(sts));
      }
      exit_scope(sts);
      semanticAnalysis(sts, ast->next);
      return;
    }
    case AST_RET: {
      TYPE ret_type = (ast->l) ? expr_type(sts, ast->l) : VOID;
      TYPE func_ret_type;
      if (!is_empty(sts->contexts)) {
        context* c = sts->contexts->head->data;
        func_ret_type = c->ret_type;
        c->returned = true;
        if (c->ret_scope < 0 || c->ret_scope > sts->cur_scope) {
          c->ret_scope = sts->cur_scope;
        }
        
      } else {
        printf("Return statement outside of a function\n");
        exit(1);
      }
      if (!matchType(ret_type, func_ret_type)) {
        printf("Wrong function return type: %d != %d\n", ret_type, func_ret_type);
        exit(1);
      }
      // printf("ret type: %d\n", ret_type);
      // return;
      break;
    }
    case AST_FCALL: {
      typecheck_fcall(sts, ast);
      break;
    }
    case AST_BLOCK: {
      enter_scope(sts);
      semanticAnalysis(sts, ast->l);
      print_symtab(stdout, currentScope(sts));
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
    default:
      break;
  }
  semanticAnalysis(sts, ast->l);
  semanticAnalysis(sts, ast->r);
  semanticAnalysis(sts, ast->next);
}

// To be deleted
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

TYPE expr_type(symtabStack* sts, AST* expr) {
  int type;
  if (expr->type == AST_OPERATOR) {
    type = typecheck_operator(sts, expr->l, expr->r);
  }
  else if (expr->type == AST_FCALL) {
    typecheck_fcall(sts, expr);
    char* f_id = expr->l->value->value;
    stEntry* f = lookup_all(sts, f_id);
    type = f->f_info->ret_type;
    if (matchType(type, VOID)) {
      printf("Function returning void in expression\n");
      exit(1);
    }
  }
  else if (expr->type == AST_ID) {
    stEntry* var = lookup_all(sts, expr->value->value);
    if (!var) {
      printf("Variable %s not declared before use\n", expr->value->value);
      exit(1);
    }
    type = var->type;
  } else {
    type = expr->value->type;
  }
  return convertType(type);
}

bool typecheck_fcall(symtabStack* sts, AST* fcall) {
  AST* arg = fcall->r->l;
  stEntry* f = lookup_all(sts, fcall->l->value->value);
  if (!f) {
    printf("Function %s not declared before use\n", fcall->l->value->value);
    exit(1);
  }
  int i = 0;
  while(arg && i < f->f_info->n_params) {
    if (!matchType(expr_type(sts, arg), f->f_info->param_types[i])) {
      printf("Function parameter is the wrong type\n");
      exit(1);
    }
    i++;
    arg = arg->next;
  }
  if (arg != NULL) {
    printf("Too many arguments given to fcall\n");
    exit(1);
  }
  else if (i < f->f_info->n_params) {
    printf("Too few arguments given to fcall\n");
    exit(1);
  }

  return true;
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
  rhs_type = expr_type(sts, rhs);

  return matchType(lhs_type, rhs_type);
}

int typecheck_operator(symtabStack* sts, AST* lhs, AST* rhs) {
  int lt = expr_type(sts, lhs);
  int rt = expr_type(sts, rhs);

  if (matchType(lt, rt)) return lt;
  else return -1;
}

void checkAST(AST* root) {
  symtabStack* st_stack = init_st_stack();

  semanticAnalysis(st_stack, root);

  printf("\nGlobal scope:\n");
  print_symtab(stdout, st_stack->s->head->data);

  // printf("Global variables %d\n", (int)currentScope(st_stack)->n);
  free_st_stack(st_stack);

}

