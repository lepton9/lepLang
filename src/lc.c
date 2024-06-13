#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer.h"

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

  Lexer* lexer = initLexer();
  readBytes(fileName, &lexer->src, &lexer->srcLen);

  lex(lexer);

  node* head = lexer->tokens->head;
  while(head->next != NULL) {
    printToken(head->data);
    head = head->next;
  }

  printf("%s\n", lexer->src);
  printf("Read %d characters\n", lexer->srcLen);

  free(lexer);

  return 0;
}
