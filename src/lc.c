#include "../include/lc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

lc* initLC() {
  lc* lepC = malloc(sizeof(lc));
  lepC->lexer = initLexer();
  lepC->parser = initParser(lepC->lexer);
  return lepC;
}

void freeLC(lc *lc) {
  freeLexer(lc->lexer);
  freeParser(lc->parser);
  free(lc);
}

void lccompile(lc *lc) {
  LList* tokens = lex(lc->lexer);
  lc->root = parse(lc->parser);
}


void readSrcFile(const char *fileName, char **buffer, int *length) {
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
  char file[8] = "test.lep"; // For debugging
  if (argc < 2) {
    printf("Pass .lep file to compile. Usage:\n");
    printf("lc <file.lep>\n");
    // return 1;
  }
  char *fileName = (argc < 2) ? file : argv[1];

  clock_t begin = clock();

  lc* lc = initLC();
  readSrcFile(fileName, &lc->lexer->src, &lc->lexer->srcLen);

  lccompile(lc);

  clock_t end = clock();
  double time = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Read %d characters\n", lc->lexer->srcLen);
  // printTokens(lc->lexer);
  printErrors(lc->lexer);
  printAST(lc->root, 0);
  // printf("%s\n", lc->lexer->src);

  printf("Took %f s\n", time);

  freeLC(lc);

  return 0;
}
