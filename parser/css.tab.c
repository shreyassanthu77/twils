#line 1 "parser/css.y"

	#include "ast.h"
	#include "lexer.h"

#line 9 "parser/css.y"
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
short yyini = 0;
short yyntoks = 11;
short yyr1[] = {
   2,   2,   1,   4,   1,   2,   4,   1,   1,   1,
   1
};
short yyr2[] = {
   0,   1,   1,   2,   3,   3,   4,   5,   6,   6,
   6
};
short yyadef[] = {
  -1,  -1,   0,   1,   2,  -1,  -1,  -1,   3,   4,
   5,  -1,  -1,  -1,   6,   7,   8,   9,  10
};
short yygdef[] = {
  -1,   1,   4,   7,   9,   5,  13
};
short yyadsp[] = {
  -3,   3, -11, -11, -11,   7,  -2,   2, -11, -11,
 -11,   4,  -8,   5, -11, -11, -11, -11, -11
};
short yygdsp[] = {
 -19, -19,  11, -19,   6, -19, -19
};
short yyact[] = {
  16,  17,  18,   2,   8,  15,  11,  12,   6,  14,
  11,  15,   3,  10
};
short yychk[] = {
   8,   9,  10,   0,   2,   8,   8,   3,   1,   4,
   8,   8,  13,  15
};
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
short yytrns[] = {
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   1,   2,
   3,   4,   5,   6,   7,   8,   9,  10
};

#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval;

int
yyparse(void* lexer, void** data)
{
	enum {
		StackSize = 100,
		ActSz = sizeof yyact / sizeof yyact[0],
	};
	struct {
		YYSTYPE val;
		int state;
	} stk[StackSize], *ps;
	int r, h, n, s, tk;
	YYSTYPE yyval;

	ps = stk;
	ps->state = s = yyini;
	tk = -1;
loop:
	n = yyadsp[s];
	if (tk < 0 && n > -yyntoks)
		tk = yytrns[yylex(lexer)];
	n += tk;
	if (n < 0 || n >= ActSz || yychk[n] != tk) {
		r = yyadef[s];
		if (r < 0)
			return -1;
		goto reduce;
	}
	n = yyact[n];
	if (n == -1)
		return -1;
	if (n < 0) {
		r = - (n+2);
		goto reduce;
	}
	tk = -1;
	yyval = yylval;
stack:
	ps++;
	if (ps-stk >= StackSize)
		return -2;
	s = n;
	ps->state = s;
	ps->val = yyval;
	goto loop;
reduce:
	ps -= yyr1[r];
	h = yyr2[r];
	s = ps->state;
	n = yygdsp[h] + s;
	if (n < 0 || n >= ActSz || yychk[n] != yyntoks+h)
		n = yygdef[h];
	else
		n = yyact[n];
	switch (r) {
	case 0:
#line 0 "parser/css.y"
		yyval = ps[1].val; return 0;
		break;
	case 1:
#line 38 "parser/css.y"
{ yyval.stylesheet = stylesheet_add_rule(ps[1].val.stylesheet, ps[2].val.rule); }
		break;
	case 2:
#line 35 "parser/css.y"
{ 
				if (data)	*data = stylesheet_new(ps[1].val.rule); 
		}
		break;
	case 3:
#line 42 "parser/css.y"
{ yyval.rule = rule_new(ps[1].val.string, ps[3].val.declaration); }
		break;
	case 4:
#line 46 "parser/css.y"
{ yyval.declaration = ps[1].val.declaration; }
		break;
	case 5:
#line 47 "parser/css.y"
{ yyval.declaration = declaration_add(ps[1].val.declaration, ps[2].val.declaration); }
		break;
	case 6:
#line 55 "parser/css.y"
{ yyval.declaration = declaration_new(ps[1].val.string, ps[3].val.value); }
		break;
	case 7:
#line 51 "parser/css.y"
{ yyval.string = ps[1].val.string; }
		break;
	case 8:
#line 59 "parser/css.y"
{ yyval.value = value_new_string(ps[1].val.string); }
		break;
	case 9:
#line 60 "parser/css.y"
{ yyval.value = value_new_number(ps[1].val.number); }
		break;
	case 10:
#line 61 "parser/css.y"
{ yyval.value = value_new_string(ps[1].val.string); }
		break;
	}
	goto stack;
}
#line 64 "parser/css.y"


