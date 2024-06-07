#include "lexer.h"
#include "ast.h"
#include "css.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexer *lexer_new(read_char_fn read_char, void *data) {
  lexer *l = malloc(sizeof(lexer));
  memset(l, 0, sizeof(lexer));
  l->read_char = read_char;
  l->data = data;
  l->line = 1;
  l->col = 1;
  l->pos = 0;
  l->peek = l->read_char(l->data);
  return l;
}

struct file_state {
  FILE *file;
  size_t len;
};

char read_char_from_file(void *data) {
  struct file_state *fs = (struct file_state *)data;
  if (fs->len == 0) {
    return EOF;
  }
  fs->len--;
  return fgetc(fs->file);
}

lexer *lexer_new_from_file(FILE *file) {
  fseek(file, 0, SEEK_END);
  size_t len = ftell(file);
  fseek(file, 0, SEEK_SET);
  struct file_state *fs = malloc(sizeof(struct file_state));
  fs->file = file;
  fs->len = len;
  return lexer_new(read_char_from_file, fs);
}

void lexer_add_free(lexer *l, void *ptr) {
  struct _free_list *fl = l->free_list;
  struct _free_list *new_fl = calloc(1, sizeof(struct _free_list));
  new_fl->ptr = ptr;
  if (fl == NULL) {
    l->free_list = new_fl;
  } else {
    new_fl->next = fl;
    l->free_list = new_fl;
  }
}

void lexer_free(lexer *l) {
  free(l);
  return;
}

char lexer_peek(lexer *l) {
  char ch = l->peek;
  return ch;
}

char lexer_next(lexer *l) {
  char ch = l->peek;
  l->pos++;
  if (ch == '\n') {
    l->line++;
    l->col = 1;
  } else {
    l->col++;
  }
  l->peek = l->read_char(l->data);
  return ch;
}

/**
 * Skip whitespace characters.
 * @param l The lexer.
 * @return True if there is a non-whitespace character left.
 */
bool lexer_skip_whitespace(lexer *l) {
  char ch = lexer_peek(l);
  while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
    lexer_next(l);
    ch = lexer_peek(l);
  }

  return ch != 0 && ch != EOF;
}

token lexer_lex_number(lexer *l);
token lexer_lex_string(lexer *l, char quote);
token lexer_lex_ident(lexer *l);

static const int ERROR = -2;
token lexer_next_token(lexer *l) {
  char ch = lexer_peek(l);
  if (ch == EOF || !lexer_skip_whitespace(l)) {
    return (token){.type = EOF, .ch = 0};
  }
  ch = lexer_peek(l);

  switch (ch) {
  case '{':
    lexer_next(l);
    return (token){.type = LSQUIRLY, .ch = ch};
  case '}':
    lexer_next(l);
    return (token){.type = RSQUIRLY, .ch = ch};
  case ':':
    lexer_next(l);
    return (token){.type = COLON, .ch = ch};
  case ';':
    lexer_next(l);
    return (token){.type = SEMICOLON, .ch = ch};
  case ',':
    lexer_next(l);
    return (token){.type = COMMA, .ch = ch};
  case '(':
    lexer_next(l);
    return (token){.type = LPAREN, .ch = ch};
  case ')':
    lexer_next(l);
    return (token){.type = RPAREN, .ch = ch};
  case '0' ... '9':
    return lexer_lex_number(l);
  case '"':
  case '\'':
    return lexer_lex_string(l, ch);
  case 'a' ... 'z':
  case 'A' ... 'Z':
  case '_':
  case '-':
  case '\\':
  case '@':
  case '#':
  case '.':
  case '$':
    return lexer_lex_ident(l);
  }

  char *msg = malloc(256);
  sprintf(msg, "Unexpected character: %c at %zu:%zu", ch, l->line, l->col);
  return (token){.type = ERROR, .err = msg};
}

token lexer_lex_number(lexer *l) {
  char buf[30];
  int n = 0;

  char ch = lexer_peek(l);
  while (ch >= '0' && ch <= '9') {
    buf[n++] = lexer_next(l);
    ch = lexer_peek(l);
  }

  if (ch == '.') {
    buf[n++] = lexer_next(l);
    ch = lexer_peek(l);
    while (ch >= '0' && ch <= '9') {
      buf[n++] = lexer_next(l);
      ch = lexer_peek(l);
    }
  }

  double d = strtod(buf, NULL);
  return (token){.type = NUMBER, .number = d};
}

token lexer_lex_string(lexer *l, char quote) {
  char buf[256];
  int n = 0;
  lexer_next(l);
  char ch = lexer_peek(l);
  while (ch != quote) {
    if (ch == EOF) {
      char *msg = malloc(256);
      sprintf(msg, "Unexpected EOF while parsing string at %zu:%zu", l->line,
              l->col);
      return (token){.type = ERROR, .err = msg};
    }
    buf[n++] = lexer_next(l);
    ch = lexer_peek(l);
  }
  lexer_next(l);
  buf[n] = 0;
  char *str = calloc(1, n + 1);
  strcpy(str, buf);
  lexer_add_free(l, str);
  return (token){.type = STRING, .string = str};
}

#define is_ident_char(ch)                                                      \
  ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' ||        \
   ch == '-' || ch == '\\' || ch == '@' || ch == '#' || ch == '.' ||           \
   ch == '$')

token lexer_lex_ident(lexer *t) {
  char buf[256];
  int n = 0;
  char ch = lexer_peek(t);
  while (is_ident_char(ch) || (ch >= '0' && ch <= '9')) {
    if (ch == '\\') {
      lexer_next(t);
      ch = lexer_peek(t);
      if (ch == EOF || ch == '\n' || ch == '\r' || ch == '\t' || ch == ' ') {
        char *msg = malloc(256);
        sprintf(msg, "Unexpected space after escape character at %zu:%zu",
                t->line, t->col);
        return (token){.type = ERROR, .err = msg};
      }
    }
    buf[n++] = lexer_next(t);
    ch = lexer_peek(t);
  }
  buf[n] = 0;
  char *str = calloc(1, n + 1);
  strcpy(str, buf);
  lexer_add_free(t, str);
  return (token){.type = IDENT, .string = str};
}

void yyerror(char *s);

int yylex(void *data) {
  lexer *lex = (lexer *)data;
  token tok = lexer_next_token(lex);
  switch (tok.type) {
  case EOF:
    return 0;
  case ERROR:
    yyerror(tok.err);
    free(tok.err);
    return 0;
  case IDENT:
    yylval.string = tok.string;
    break;
  case NUMBER:
    yylval.number = tok.number;
    break;
  case STRING:
    yylval.string = tok.string;
    break;
  default:
    break;
  }
  return tok.type;
}
