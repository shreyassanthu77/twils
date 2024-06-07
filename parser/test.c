#include "css.tab.h"
#include "lexer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  lexer *lexer = lexer_new_from_file(file);
  Stylesheet *stylesheet = {0};
  int v = yyparse(lexer, (void **)&stylesheet);
  if (v != 0) {
    printf("Error: %d\n", v);
  }

  assert(stylesheet != NULL);
  assert(stylesheet->rules != NULL);
  assert(stylesheet->rules->selector != NULL);
  assert(strcmp(stylesheet->rules->selector, "main") == 0);
  assert(stylesheet->rules->declarations != NULL);
  assert(stylesheet->rules->declarations->property != NULL);
  assert(strcmp(stylesheet->rules->declarations->property, "--tests;") == 0);
  assert(stylesheet->rules->declarations->value.type == VALUE_NUMBER);
  assert(stylesheet->rules->declarations->value.number == 2);
  assert(stylesheet->rules->next == NULL);

  lexer_free(lexer);
  fclose(file);
  return 0;
}

void yyerror(const char *s) {
  printf("Error: %s\n", s);
  exit(1);
}
