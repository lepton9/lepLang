#include "../include/ssair.h"
#include <string.h>

ssa* init_ssa() {
  ssa* ssaIR = malloc(sizeof(ssa));
  return ssaIR;
}

function* create_function(const char* name, const int param_n) {
  function* f = malloc(sizeof(function));
  f->name = strdup(name);
  f->param_n = param_n;
  f->params = malloc(param_n * sizeof(var*));
  for (int i = 0; i < param_n; i++) f->params[i] = malloc(sizeof(var));
  return f;
}
instruction* create_instruction(opcode op, int dest, int src1, int src2) {

}
basic_block* create_block(int id) {

}

ssa* generate_ssair(AST* root) {
  ssa* ssair = init_ssa();
  ssa_node* back = ssair->entry;
  symtabStack* sts = init_st_stack();
  AST* node = root;
  while(node) {
    ssa_node* ssaNode;
    switch (node->type) {
      case AST_FUNCTION:
        ssaNode = addSSAfunc(ssair, sts, node);
        break;
      case AST_VARIABLE:
        ssaNode = addSSAvar(ssair, sts, node);
        break;
      case AST_ASSIGNMENT:
        ssaNode = addSSAvar(ssair, sts, node);
        break;
    }
    if (back) {
      back->next = ssaNode;
      back = back->next;
    } else {
      back = ssaNode;
    }
    node = node->next;
  }
  return ssair;
}

ssa_node* addSSAfunc(ssa* ssa, symtabStack* sts, AST* ast) {
  enter_scope(sts);
  ssa_node* ssaNode = malloc(sizeof(ssa_node));
  // ssaNode->value.f = create_function(const char *name, const int param_n);
  exit_scope(sts);
  return ssaNode;
}

ssa_node* addSSAvar(ssa* ssa, symtabStack* sts, AST* ast) {
  ssa_node* ssaNode = malloc(sizeof(ssa_node));
  // ssaNode->value.i = create_instruction(opcode op, int dest, int src1, int src2);
  return ssaNode;
}

ssa_node* addSSAassign(ssa* ssa, symtabStack* sts, AST* ast) {
  ssa_node* ssaNode = malloc(sizeof(ssa_node));
  // ssaNode->value.i = create_instruction(opcode op, int dest, int src1, int src2);
  return ssaNode;
}

