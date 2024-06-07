%{
	#include "ast.h"
	#include "lexer.h"
%}

%token LSQUIRLY RSQUIRLY COLON SEMICOLON COMMA LPAREN RPAREN
%token IDENT NUMBER STRING

%union {
	Stylesheet *stylesheet;
	Rule *rule;
	Declaration *declaration;
	Value value;
	char ch;
	char *string;
	double number;
}

%token <string> IDENT STRING
%token <number> NUMBER

%type <stylesheet> stylesheet
%type <rule> rule
%type <declaration> declarationList
%type <declaration> declaration
%type <string> selector
%type <value> value


%start stylesheet

%%

stylesheet
		: rule { 
				if (data)	*data = stylesheet_new($1); 
		}
		| stylesheet rule { $$ = stylesheet_add_rule($1, $2); } 
		;

rule
	:	selector LSQUIRLY declarationList RSQUIRLY { $$ = rule_new($1, $3); } 
	;

declarationList
	:	declaration { $$ = $1; } 
	| declarationList declaration { $$ = declaration_add($1, $2); } 
	;

selector
	:	IDENT { $$ = $1; } 
	;

declaration
	:	IDENT COLON value SEMICOLON { $$ = declaration_new($1, $3); } 
	;

value
	:	IDENT { $$ = value_new_string($1); } 
	| NUMBER { $$ = value_new_number($1); } 
	| STRING { $$ = value_new_string($1); } 
	;

%%

