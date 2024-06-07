#ifndef AST_H
#define AST_H

typedef struct Value {
  enum { VALUE_STRING, VALUE_NUMBER } type;

  union {
    char *string;
    double number;
  };
} Value;

typedef struct Declaration {
  char *property;
  Value value;

  struct Declaration *next;
} Declaration;

typedef struct Rule {
  char *selector;
  Declaration *declarations;

  struct Rule *next;
} Rule;

typedef struct Stylesheet {
  Rule *rules;
} Stylesheet;

Stylesheet *stylesheet_new(Rule *rule);
Stylesheet *stylesheet_add_rule(Stylesheet *stylesheet, Rule *rule);

Rule *rule_new(char *selector, Declaration *declaration);

Declaration *declaration_new(char *property, Value value);
Declaration *declaration_add(Declaration *declaration, Declaration *next);

Value value_new_string(char *string);
Value value_new_number(double number);

#endif
