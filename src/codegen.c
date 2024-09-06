#include "../include/codegen.h"
#include <string.h>

asmf* initAsmfile() {
  asmf* af = malloc(sizeof(asmf));
  af->size = 1000;
  af->n = 0;
  af->file = malloc(af->size * sizeof(char));
  return af;
}

void freeAsmfile(asmf* af) {
  free(af->file);
  free(af);
}

void append_line(asmf* af, const char* line) {
  int len = strlen(line);
  if (af->size - af->n < len + 2) {
    af->size *= 2;
    af->file = realloc(af->file, af->size);
  }
  // strcat(af->file, line);
  memcpy(af->file + af->n, line, len);
  af->n += len;
  if (line[len-1] != '\n') {
    // strcat(af->file, "\n");
    af->file[af->n] = '\n';
    af->n++;
  }
  af->file[af->n] = '\0';
}

