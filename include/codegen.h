#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdlib.h>

typedef struct {
  char* file;
  size_t size;
  size_t n;
} asmf;

asmf* initAsmfile();
void freeAsmfile(asmf* af);
void append_line(asmf* af, const char* line);

void compile_expr(); // Val to rax
void compile_var(); // Push to stack
void compile_add();
void compile_neg();
void compile_fcall(); // Stack frame

#endif
