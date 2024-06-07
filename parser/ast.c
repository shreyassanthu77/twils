#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

Stylesheet *stylesheet_new(Rule *rule) {
  Stylesheet *stylesheet = calloc(1, sizeof(Stylesheet));
  stylesheet->rules = rule;
  return stylesheet;
}
Stylesheet *stylesheet_add_rule(Stylesheet *stylesheet, Rule *rule) {
  Rule *rules = stylesheet->rules;
  while (rules->next != NULL) {
    rules = rules->next;
  }
  rules->next = rule;
  return stylesheet;
}

Rule *rule_new(char *selector, Declaration *declaration) {
  Rule *rule = calloc(1, sizeof(Rule));
  rule->selector = selector;
  rule->declarations = declaration;
  return rule;
}

Declaration *declaration_new(char *property, Value value) {
  Declaration *declaration = calloc(1, sizeof(Declaration));
  declaration->property = property;
  declaration->value = value;
  return declaration;
}
Declaration *declaration_add(Declaration *declaration, Declaration *next) {
  Declaration *declarations = declaration;
  while (declarations->next != NULL) {
    declarations = declarations->next;
  }
  declarations->next = next;
  return declaration;
}

Value value_new_string(char *string) {
  Value value = {.type = VALUE_STRING, .string = string};
  return value;
}
Value value_new_number(double number) {
  Value value = {.type = VALUE_NUMBER, .number = number};
  return value;
}
