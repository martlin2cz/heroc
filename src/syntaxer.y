%{

#include "../src/syntaxer-headers.h"

%}
/* settings */

%output "gen/syntaxer.c"
%defines "gen/syntaxer.h"



%union {
	long number;
	char* string;
	int keyword;
	int oper;
	struct ast_node_t* child;
}

%parse-param {struct ast_node_t** root}

%start program

/* tokens (terminals) */
/* atomic values */
%token<number> 	NT_NUMBER		1001
%token<string> 	NT_STRING		1002
%token<string> 	NT_IDENTIFIER	1003
%token 			NT_SEMICOLON	1101

/* special tokens */
%token	TT_MINUS 				0001
%token	TT_STAR 				0002
%token	TT_INCREMENT 			0003
%token	TT_DECREMENT 			0004
%token	TT_NORMAL_LEFT_BRA		0011
%token	TT_NORMAL_RIGHT_BRA		0012


%token<child>	NTO_PLUS 		2101
%token<child>	NTO_MINUS 		2102
%token<child>	NTO_DIVIDE 		2103
%token<child>	NTO_MODULO		2104

/* (nonterminals) */
%type<child> program statement expression
 
 /* asociativity and priority (precendence) */
 /* http://www.linuxsoft.cz/article.php?id_article=486 */
%left NTO_MINUS NTO_PLUS
%left NTO_TIMES NTO_DIVIDE NTO_MODULO 
%left NTP_UNARY_MINUS      
     
%%

/* rules */

program: 
	statement	{	*root = $$ = $1;	SYNTAXER_LOG("statement %p", $1);	}
;
 
statement:
	  NT_SEMICOLON				{	*root = $$ = NULL;	SYNTAXER_LOG("empty statement");	}
	| expression NT_SEMICOLON	{	*root = $$ = $1;	SYNTAXER_LOG("expresion statement %p ;", $1);	}
;
 
expression:   
  	  expression NTO_PLUS expression	{ *root = $$ = create_binary(NTO_PLUS, $1, $3); 	SYNTAXER_LOG("plus  of %p %p -> %p", $1, $3, $$);	}
	| expression TT_MINUS expression	{ *root = $$ = create_binary(NTO_MINUS, $1, $3);	SYNTAXER_LOG("minus of %p %p -> %p", $1, $3, $$);	}
	| expression TT_STAR expression		{ *root = $$ = create_binary(NTO_TIMES, $1, $3);	SYNTAXER_LOG("times of %p %p -> %p", $1, $3, $$);	}
	| expression NTO_DIVIDE expression	{ *root = $$ = create_binary(NTO_DIVIDE, $1, $3);	SYNTAXER_LOG("div   of %p %p -> %p", $1, $3, $$);	}
	| TT_MINUS expression %prec NTP_UNARY_MINUS		{ *root = $$ = create_unary(NTO_MINUS, $2);		SYNTAXER_LOG("unary minus of %p -> %p", $2, $$);	}
	
	| TT_NORMAL_LEFT_BRA expression TT_NORMAL_RIGHT_BRA 		{*root = $$ = $2;							SYNTAXER_LOG("( %p ) -> %p", $2, $$);	}
	| NT_NUMBER			{ *root = $$ = create_number($1);			SYNTAXER_LOG("number %ld -> %p", $1, $$);	}
	| NT_IDENTIFIER		{ *root = $$ = create_identifier($1);		SYNTAXER_LOG("identifier %s -> %p", $1, $$);}
;

%%

#include "../src/syntaxer-impl.c"