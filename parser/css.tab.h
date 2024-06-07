#ifndef Y_TAB_H_
#define Y_TAB_H_
#line 1 "parser/css.y"

	#include "ast.h"
	#include "lexer.h"
typedef union {
	Stylesheet *stylesheet;
	Rule *rule;
	Declaration *declaration;
	Value value;
	char ch;
	char *string;
	double number;
} yyunion;
#define YYSTYPE yyunion
#define LSQUIRLY 128
#define RSQUIRLY 129
#define COLON 130
#define SEMICOLON 131
#define COMMA 132
#define LPAREN 133
#define RPAREN 134
#define IDENT 135
#define NUMBER 136
#define STRING 137
int yyparse(void* lexer, void** data);
#ifndef YYSTYPE
#define YYSTYPE int
#endif
extern YYSTYPE yylval;
#endif
