#include "../include/codegen.h"
#include <stdio.h>
#include <string.h>

asmf* initAsmfile(const char* fname) {
  asmf* af = malloc(sizeof(asmf));
  af->file = fopen(fname, "w");

  af->size = 1000;
  af->n = 0;
  af->out = malloc(af->size * sizeof(char));

  return af;
}

void freeAsmfile(asmf* af) {
  fclose(af->file);
  free(af->file);
  free(af->out);
  free(af);
}

void append_line(asmf* af, const char* line) {
  int len = strlen(line);
  if (af->size - af->n < len + 2) {
    af->size *= 2;
    af->out = realloc(af->out, af->size);
  }
  // strcat(af->out, line);
  memcpy(af->out + af->n, line, len);
  af->n += len;
  if (line[len-1] != '\n') {
    // strcat(af->out, "\n");
    af->out[af->n] = '\n';
    af->n++;
  }
  af->out[af->n] = '\0';
}


void init_stackframe(asmf* af, const size_t size) {
  fprintf(af->file, "pushq %%rbp\n");
  fprintf(af->file, "movq %%rsp, %%rbp\n");
  fprintf(af->file, "subq $%d, %%rsp\n", (int)size);
}

void exit_stackframe(asmf* af) {
  fprintf(af->file, "movq %%rbp, %%rsp\n");
  fprintf(af->file, "popq %%rpb\n");
}

void compile_global_var(asmf* af, stEntry* e, void* value) {
    fprintf(af->file, "  .globl %s\n", e->name);
    fprintf(af->file, "  .data\n");
    fprintf(af->file, "  .align 4\n");
    fprintf(af->file, "  .type %s, @object\n", e->name);
    fprintf(af->file, "  .size %s, %d\n", e->name, (int)e->size); // 4 for int
    fprintf(af->file, "%s:\n", e->name);
    if (e->type == CHAR) {
      fprintf(af->file, "  .byte %d\n", (value) ? *(int*)value : 0);
    } 
    else if (e->type == STR) {
      fprintf(af->file, "  .ascii \"%s\"\n", (value) ? *(char**)value : "");
    } 
    else {
      fprintf(af->file, "  .long %d\n", (value) ? *(int*)value : 0);
    }
  fprintf(af->file, "\n");
}

void compile_func_def(asmf* af, symtabStack* sts, AST* ast) {
  size_t stackframe_size = 2048; // Calculate needed size
  fprintf(af->file, "%s:\n", "funcname");
  init_stackframe(af, stackframe_size);

  fprintf(af->file, "movq $%d, %%rax\n", 0); // Ret value
  exit_stackframe(af);
  fprintf(af->file, "ret\n");
}



