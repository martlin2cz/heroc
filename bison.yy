%{
#include <stdio.h>
#include "bison-headers.h"
#include "bison.h"

%}

%output "bison.c"
%defines "bison.h"

%parse-param {syntax_node_t** root}


%token<number>	TK_NUMBER	42
%token<string>	TK_STRING	'@'

%token<string> TK_VARIABLE	'_'
%token TK_COMMENT	'#'

%token	TK_COMA	','
%token	TK_SEMICOLON	';'
%token	TK_CURLY_BRACKET_OPEN	'{'
%token	TK_CURLY_BRACKET_CLOSE	'}'

%token TK_ROUND_BRACKET_OPEN	'('
%token TK_ROUND_BRACKET_CLOSE	')'

%token TK_OPERATOR_PLUS	0x1011
%token TK_OPERATOR_MINUS	0x1012
%token TK_OPERATOR_TIMES 0x1013
%token TK_OPERATOR_DIVIDE 0x1014
%token TK_OPERATOR_MODULO 0x1015
%token TK_OPERATOR_INCREMENT 0x1016
%token TK_OPERATOR_DECREMENT 0x1017

%token TK_OPERATOR_EQUAL 0x1021
%token TK_OPERATOR_NOT_EQUAL 0x1022
%token TK_OPERATOR_LESS_THAN 0x1023
%token TK_OPERATOR_GREATER_THAN 0x1024
%token TK_OPERATOR_LESS_EQUAL 0x1025
%token TK_OPERATOR_GREATER_EQUAL 0x1026

%token TK_OPERATOR_NOT 0x1031
%token TK_OPERATOR_AND 0x1032
%token TK_OPERATOR_OR 0x1033

%token TK_OPERATOR_BIT_NOT 0x1041
%token TK_OPERATOR_BIT_AND 0x1042
%token TK_OPERATOR_BIT_OR 0x1043
%token TK_OPERATOR_BIT_XOR 0x1044
%token TK_OPERATOR_SHIFT_LEFT 0x1045
%token TK_OPERATOR_SHIFT_RIGHT 0x1046

%token<operator> TK_OPERATOR_ASSIGNMENT 0x1000

%token TK_KEYW_IF	'?'
%token TK_KEYW_ELSE ':'
%token TK_KEYW_FOR	'!'
%token TK_KEYW_DO	'd'
%token TK_KEYW_WHILE	'w'
%token TK_KEYW_CONTINUE	'c'
%token TK_KEYW_BREAK	'b'
						  
%token TK_KEYW_PRINT	'p'
%token TK_KEYW_SCAN	's'

%token TK_MISSING	'x'
%token TK_UNKNOWN 'X'

%type <node> expression subexpression atom 
%type <node> subexpr1 subexpr2 subexpr3 subexpr4 subexpr5 subexpr6 subexpr7 subexpr8 subexpr9 subexpr10  subexpr11 subexpr12 subexpr14
%type<node> keyword naked_statement statement statements compound_statement program
%type <node> print_statement scan_statement if_statement for_statement do_while_statement while_statement

%start program


%%
expression:
	subexpression {}
;
subexpression:
	subexpr14 {}
;

subexpr1:
		atom {}
	|	TK_CURLY_BRACKET_OPEN subexpr1 TK_CURLY_BRACKET_CLOSE {
			$$ = $2; 
			VERB("Expr (#1) in brackets", $$, "( %p )", $2); 
	}
;

subexpr2:
		subexpr1 {}
	|	TK_OPERATOR_PLUS subexpr1 {
			UNARY_OP($$, 2, TK_OPERATOR_PLUS, $2);
	}
	|	TK_OPERATOR_MINUS subexpr1 {
			UNARY_OP($$, 2, TK_OPERATOR_MINUS, $2);
	}
	|	TK_OPERATOR_INCREMENT TK_VARIABLE {
			UNARY_OP($$, 2, TK_OPERATOR_PRE_INC, create_variable($2));
	}
	|	TK_OPERATOR_DECREMENT TK_VARIABLE {
			UNARY_OP($$, 2, TK_OPERATOR_PRE_DEC, create_variable($2));
	}
	|	TK_VARIABLE TK_OPERATOR_INCREMENT {
			UNARY_OP($$, 2, TK_OPERATOR_INCREMENT, create_variable($1));
	}
	|	TK_VARIABLE TK_OPERATOR_DECREMENT {
			UNARY_OP($$, 2, TK_OPERATOR_DECREMENT, create_variable($1));
	}
	|	TK_OPERATOR_NOT subexpr1 {
			UNARY_OP($$, 2, TK_OPERATOR_NOT, $2);
	}
	|	TK_OPERATOR_BIT_NOT subexpr1 {
			UNARY_OP($$, 2, TK_OPERATOR_BIT_NOT, $2);
	}
;
subexpr3:
		subexpr2 {}
	|	subexpr2 TK_OPERATOR_TIMES subexpr3 {
			BINARY_OP($$,3, TK_OPERATOR_TIMES, $1, $3);
	}
	|	subexpr2 TK_OPERATOR_DIVIDE subexpr3 {
			BINARY_OP($$,3, TK_OPERATOR_DIVIDE, $1, $3);
	}
	|	subexpr2 TK_OPERATOR_MODULO subexpr3 {
			BINARY_OP($$,3, TK_OPERATOR_MODULO, $1, $3);
	}
;
subexpr4:
		subexpr3 {}
	|	subexpr3 TK_OPERATOR_PLUS subexpr4 {
			BINARY_OP($$,4, TK_OPERATOR_PLUS, $1, $3);
	}
	|	subexpr3 TK_OPERATOR_MINUS subexpr4 {
			BINARY_OP($$,4, TK_OPERATOR_MINUS, $1, $3);
	}
;
subexpr5:
		subexpr4 {}
	|	subexpr4 TK_OPERATOR_SHIFT_LEFT subexpr5 {
			BINARY_OP($$,5, TK_OPERATOR_SHIFT_LEFT, $1, $3);
	}
	|	subexpr4 TK_OPERATOR_SHIFT_RIGHT subexpr5 {
			BINARY_OP($$,5, TK_OPERATOR_SHIFT_RIGHT, $1, $3);
	}
;
subexpr6:
		subexpr5 {}
	|	subexpr5 TK_OPERATOR_LESS_THAN subexpr6 {
			BINARY_OP($$,6, TK_OPERATOR_LESS_THAN, $1, $3);
	}	
	|	subexpr5 TK_OPERATOR_GREATER_THAN subexpr6 {
			BINARY_OP($$,6, TK_OPERATOR_GREATER_THAN, $1, $3);
	}	
	|	subexpr5 TK_OPERATOR_LESS_EQUAL subexpr6 {
			BINARY_OP($$,6, TK_OPERATOR_LESS_EQUAL, $1, $3);
	}
	|	subexpr5 TK_OPERATOR_GREATER_EQUAL subexpr6 {
			BINARY_OP($$,6, TK_OPERATOR_GREATER_EQUAL, $1, $3);
	}
;
subexpr7:
		subexpr6 {}
	|	subexpr6 TK_OPERATOR_EQUAL  subexpr7 {
			BINARY_OP($$,7, TK_OPERATOR_EQUAL, $1, $3);
	}
	|	subexpr6 TK_OPERATOR_NOT_EQUAL subexpr7 {
			BINARY_OP($$,7, TK_OPERATOR_NOT_EQUAL, $1, $3);
	}	
;
subexpr8:
		subexpr7 {}
	|	subexpr7 TK_OPERATOR_BIT_AND subexpr8 {
			BINARY_OP($$,8, TK_OPERATOR_BIT_AND, $1, $3);
	}

;
subexpr9:
		subexpr8 {}
	|	subexpr8 TK_OPERATOR_BIT_XOR subexpr9 {
			BINARY_OP($$,9, TK_OPERATOR_BIT_XOR, $1, $3);
	}

;
subexpr10:
		subexpr9 {}
	|	subexpr9 TK_OPERATOR_BIT_OR subexpr10 {
			BINARY_OP($$,10, TK_OPERATOR_BIT_OR, $1, $3);
	}

;
subexpr11:
		subexpr10 {}
	|	subexpr10 TK_OPERATOR_AND subexpr11 {
			BINARY_OP($$,11, TK_OPERATOR_AND, $1, $3);
	}

;
subexpr12:
		subexpr11 {}
	|	subexpr11 TK_OPERATOR_OR subexpr12 {
		BINARY_OP($$,12, TK_OPERATOR_OR, $1, $3);
	}
;

subexpr14:
		subexpr12 {}
	|	TK_VARIABLE TK_OPERATOR_ASSIGNMENT expression { 
			$$ = create_assignment($2, create_variable($1), $3); 
			VERB("assignment (#14)", $$, "set %s (with operator %x) to %p", $1, $2, $3); 
		}
;


atom:
		TK_NUMBER { 
			$$ = create_number($1);	
			VERB("Number", $$, "%d", $1);
		}

	|	TK_VARIABLE { 
			$$ = create_variable($1);	
			VERB("Variable use", $$, "%s", $1);
		}	
;

/***************************************************************/


print_statement:
		TK_KEYW_PRINT TK_ROUND_BRACKET_OPEN TK_STRING[str] TK_ROUND_BRACKET_CLOSE {
			$$ = create_print(create_text($[str]), NULL);
			VERB("print string", $$, "print(\"%s\")", $[str]);
		}

	|	TK_KEYW_PRINT TK_ROUND_BRACKET_OPEN expression[ex] TK_ROUND_BRACKET_CLOSE {
		$$ = create_print(NULL, $[ex]);
		VERB("print expression", $$, "print(%p)", $[ex]);
		}

	| TK_KEYW_PRINT TK_ROUND_BRACKET_OPEN TK_STRING[format] TK_COMA expression[ex] TK_ROUND_BRACKET_CLOSE {
			$$ = create_print(create_text($[format]), $[ex]);
			VERB("print formated expr", $$, "print(\"%s\", %p)", $[format], $[ex]);
		}
;

scan_statement:
		TK_KEYW_SCAN TK_ROUND_BRACKET_OPEN TK_VARIABLE[var] TK_ROUND_BRACKET_CLOSE {
			$$ = create_scan(create_variable($[var]));
			VERB("scan", $$, "%s := scan()", $[var]);
		}
;

if_statement:
		TK_KEYW_IF TK_ROUND_BRACKET_OPEN expression[cond] TK_ROUND_BRACKET_CLOSE statement[cmd] {
			$$ = create_if($[cond], $[cmd], NULL);
			VERB("simple if", $$, "if %p then %p", $[cond], $[cmd]);
		}
		
	|	TK_KEYW_IF TK_ROUND_BRACKET_OPEN expression[cond] TK_ROUND_BRACKET_CLOSE statement[cmd] TK_KEYW_ELSE statement[cmdelse] {
			$$ = create_if($[cond], $[cmd], $[cmdelse]);
			VERB("if-else", $$, "if %p then %p else %p", $[cond], $[cmd], $[cmdelse]);
		}
;

for_statement:
		TK_KEYW_FOR TK_ROUND_BRACKET_OPEN expression[init] TK_SEMICOLON expression[cond] TK_SEMICOLON expression[inc] TK_ROUND_BRACKET_CLOSE statement[cmd] {
			$$ = create_for($[init], $[cond], $[inc], $[cmd]);
			VERB("for", $$, "for %p until %p step %p do %p", $[init], $[cond], $[inc], $[cmd]);
		}
;

do_while_statement:
		TK_KEYW_DO statement[cmd] TK_KEYW_WHILE TK_ROUND_BRACKET_OPEN expression[cond] TK_ROUND_BRACKET_CLOSE {
			$$ = create_do_while($[cond], $[cmd]);
			VERB("do-while", $$, "do %p while %p", $[cmd], $[cond]);
		}
;

while_statement:
		TK_KEYW_WHILE TK_ROUND_BRACKET_OPEN expression[cond] TK_ROUND_BRACKET_CLOSE statement[cmd] {
			$$ = create_while($[cond], $[cmd]);
			VERB("while", $$, "while %p do  %p", $[cond], $[cmd]);
		}
;

keyword:
		TK_KEYW_CONTINUE {
			$$ = create_keyword(CONTINUE);
			VERB("continue", $$, "(keyword)");
		}
	|	TK_KEYW_BREAK {
			$$ = create_keyword(BREAK);
			VERB("break", $$, "(keyword)");
		}
;

/**********************************************************************/


statement:
		naked_statement TK_SEMICOLON {
			$$ = $1;
		}
	|	TK_SEMICOLON {
			$$ = create_commands(NULL, NULL);
			VERB("empty statement", $$, ";");
		}
	|	compound_statement
	| if_statement
	|	for_statement
	|	while_statement
	| do_while_statement

;

naked_statement:
		expression
	|	scan_statement
	|	print_statement
	|	keyword


compound_statement:
		TK_CURLY_BRACKET_OPEN statements[cmd] TK_CURLY_BRACKET_CLOSE {  
			$$ = $[cmd];
			VERB("compound statements", $$, "{ %p }", $[cmd]);
		}
;
statements:
		%empty { 
			$$ = NULL;
		}
	|	statement statements {
			$$ = create_commands($1, $2);
			VERB("statements", $$, "%p %p ...", $1, $2);
		}
;
program:
		compound_statement {
			*root = $$;
		}
;

%%


void yyerror(struct syntax_node_t **n, char *s) {
  fprintf(stderr, "Syntax error at %p and: %s\n", n, s);
}
//int yyerror(char *s) {
//  fprintf(stderr, "Syntax error: %s\n", s);
//	return 1;
//}

