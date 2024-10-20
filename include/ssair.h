#ifndef SSAIR_H
#define SSAIR_H

#include "../include/ast.h"
#include "../include/symtab.h"

typedef enum {
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_LOAD,
  OP_STORE,
  OP_PHI,
  OP_RET
} opcode;

typedef struct {
  char* name;
  int version;
} var;

typedef enum {
  SSA_VAR,
  SSA_CONST
} operand_type;

typedef struct operand {
  operand_type op_type;
  TYPE type;
  union {
    var var;
    void* constant;
  } value;
} operand;

typedef struct instruction {
  opcode op;
  operand dest;
  operand src1;
  operand src2;
  struct instruction* next;
} instruction;

typedef struct basic_block {
  int block_id;
  instruction* instructions;
  // struct basic_block** successors;
  // struct basic_block** predecessors;
  struct basic_block* next;
} basic_block;

typedef struct {
  char* name;
  var** params;
  int param_n;
  basic_block* entry_block;
} function;

typedef enum {
  GLOBAL_VAR,
  FUNCTION
} node_type;

typedef struct ssa_node {
  node_type type;
  union {
    function* f;
    instruction* i;
  } value;
  struct ssa_node* next;
} ssa_node;

typedef struct {
  ssa_node* entry;
} ssa;

ssa* generate_ssair(AST* root);

char* var_name(const char* name, const int version); // x, 1 -> x_1

ssa* init_ssa();
function* create_function(const char* name, const int param_n);
instruction* create_instruction(opcode op, char* dest, char* src1, char* src2);
basic_block* create_block(int id);

ssa_node* addSSA_func(ssa* ssa, symtabStack* sts, AST* ast);
ssa_node* addSSA_var_g(ssa* ssa, symtabStack* sts, AST* ast);
ssa_node* addSSA_assign_g(ssa* ssa, symtabStack* sts, AST* ast);

#endif

