#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include <stdlib.h>
#include "../include/symtab.h"

typedef struct {
  char name[3];
  int value;
  char in_use;
} reg;

typedef struct {
  FILE* file;
  char* out;
  size_t size;
  size_t n;
} asmf;


asmf* initAsmfile(const char* fname);
void freeAsmfile(asmf* af);
void append_line(asmf* af, const char* line);

char* inst(const char* instruction, size_t size);

void init_stackframe(asmf* af, const size_t size);
void exit_stackframe(asmf* af);

void set_var_value(asmf* af, stEntry* e, void* value);
void compile_expr(); // Val to rax
void compile_var(asmf* af, stEntry* e);
void compile_add();
void compile_neg();
void compile_fcall(); // Stack frame

void compile_global_var(asmf* af, stEntry* e, void* value);
void compile_func_def(asmf* af, symtabStack* sts, AST* ast);

#endif
