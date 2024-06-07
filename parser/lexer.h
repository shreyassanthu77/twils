#ifndef LEXER_H_
#define LEXER_H_
#include <stdio.h>

typedef char (*read_char_fn)(void *data);

struct _free_list {
  void *ptr;
  struct _free_list *next;
};

typedef struct {
  read_char_fn read_char;
  void *data;
  struct _free_list *free_list;
  size_t line;
  size_t col;
  size_t pos;
  char peek;
} lexer;

typedef struct {
  int type;
  union {
    char ch;
    char *string;
    double number;
    char *err;
  };
} token;

lexer *lexer_new(read_char_fn read_char, void *data);

lexer *lexer_new_from_file(FILE *file);

void lexer_add_free(lexer *l, void *ptr);

void lexer_free(lexer *l);

token lexer_next_token(lexer *l);

int yylex(void *data);
#endif
