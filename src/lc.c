#include "../include/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void readBytes(const char *fileName, char **buffer, int *length) {
  FILE *f = fopen(fileName, "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);
    *buffer = malloc(*length);
    if (*buffer) {
      fread(*buffer, 1, *length, f);
    }
    fclose(f);
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Pass .lep file to compile. Usage:\n");
    printf("lc <file.lep>\n");
    return 1;
  }
  char *fileName = argv[1];

  clock_t begin = clock();

  Lexer *lexer = initLexer();
  readBytes(fileName, &lexer->src, &lexer->srcLen);

  lex(lexer);

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;

  printTokens(lexer);
  printErrors(lexer);

  printf("%s\n", lexer->src);
  printf("Read %d characters\n", lexer->srcLen);
  printf("Took %f s\n", time);

  freeLexer(lexer);

  return 0;
}
