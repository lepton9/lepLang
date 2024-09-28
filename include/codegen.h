#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include <stdlib.h>
#include "../include/symtab.h"
#include "../include/regalloc.h"


typedef struct {
  FILE* file;
  char* out;
  size_t size;
  size_t n;
} asmf;

asmf* initAsmfile(const char* fname);
void freeAsmfile(asmf* af);
void append_line(asmf* af, const char* line);

char* inst(const char* instruction, const size_t size);

void init_stackframe(asmf* af, const size_t size);
void exit_stackframe(asmf* af);

void set_var_value(asmf* af, stEntry* e, void* value);
void compile_expr(asmf* af, symtabStack* sts, AST* expr, int typeSize);
void compile_var(asmf* af, stEntry* e);
void compile_add();
void compile_neg();
void compile_param(asmf* af, stEntry* e, const int regI);
void compile_fcall(); // Stack frame
void compile_ret(asmf* af, symtabStack* sts, AST* expr, int typeSize);

void compile_global_var(asmf* af, stEntry* e, void* value);
void compile_func_def(asmf* af, symtabStack* sts, stEntry* f);

#endif
