%{

#include "../src/syntaxer-headers.h"

%}
/* settings */

%output "gen/syntaxer.c"
%defines "gen/syntaxer.h"

/*
 expression TT_PLUS expression	%prec NTO_PLUS		{ *root = $$ = create_binary(NTO_PLUS, $1, $3); 	SYNTAXER_LOG("plus  of %p %p -> %p", $1, $3, $$);	}
	| expression TT_MINUS expression %prec NTO_MINUS	{ *root = $$ = create_binary(NTO_MINUS, $1, $3);	SYNTAXER_LOG("minus of %p %p -> %p", $1, $3, $$);	}
	| expression TT_STAR expression	%prec NTO_TIMES		{ *root = $$ = create_binary(NTO_TIMES, $1, $3);	SYNTAXER_LOG("times of %p %p -> %p", $1, $3, $$);	}
	| expression NTO_DIVIDE expression	{ *root = $$ = create_binary(NTO_DIVIDE, $1, $3);	SYNTAXER_LOG("div   of %p %p -> %p", $1, $3, $$);	}
	| expression NTO_MODULO expression	{ *root = $$ = create_binary(NTO_MODULO, $1, $3);	SYNTAXER_LOG("mod   of %p %p -> %p", $1, $3, $$);	}
	
	| TT_MINUS expression 		{ *root = $$ = create_unary(NTO_MINUS, $2);		SYNTAXER_LOG("unary minus of %p -> %p", $2, $$);	}
	| TT_PLUS expression 		{ *root = $$ = create_unary(NTO_PLUS, $2);		SYNTAXER_LOG("unary plus of %p -> %p", $2, $$);	}
	
	| TT_INCREMENT place %prec NTO_PRE_INCREMENT	{ *root = $$ = create_unary(NTO_PRE_INCREMENT, $2);	SYNTAXER_LOG("pre incrementation of %p -> %p", $2, $$);	}
	| TT_DECREMENT place %prec NTO_PRE_DECREMENT	{ *root = $$ = create_unary(NTO_PRE_DECREMENT, $2);	SYNTAXER_LOG("pre decrementation of %p -> %p", $2, $$);	}
	| place TT_INCREMENT 	{ *root = $$ = create_unary(NTO_POST_INCREMENT, $1);	SYNTAXER_LOG("post incrementation of %p -> %p", $1, $$);	}
	| place TT_DECREMENT 	{ *root = $$ = create_unary(NTO_POST_DECREMENT, $1);	SYNTAXER_LOG("post decrementation of %p -> %p", $1, $$);	}
	*/

%union {
	long number;
	char* string;
	int keyword;
	int oper;
	struct ast_node_t* child;
}

%parse-param {struct ast_node_t** root}

%start statement

/* tokens (terminals) */
/* lexical tokens (not in use in ast) */
%token	TT_MINUS 				0001
%token	TT_PLUS 				0002
%token	TT_STAR 				0003
%token	TT_INCREMENT			0004
%token	TT_DECREMENT			0005
%token	TT_ASSIGNMENT			0011

/* special characters */
%token	TT_COMMA				0111
%token 	TT_SEMICOLON			0112
%token	TT_QUESTION_MARK		0113
%token	TT_COLON				0114

/* brackets */
%token	TT_NORMAL_LEFT_BRA		0211
%token	TT_NORMAL_RIGHT_BRA		0212
%token	TT_BLOCK_LEFT_BRA		0213
%token	TT_BLOCK_RIGHT_BRA		0214
%token	TT_INDEX_LEFT_BRA		0215
%token	TT_INDEX_RIGHT_BRA		0216

/* atomic values */
%token<number> 	NT_NUMBER		1001
%token<string> 	NT_STRING		1002
%token<string> 	NT_IDENTIFIER	1003



/* specials */
%token<child> NTS_TYPEDEF
%token<child> NTS_ASSIGNMENT
%token<child> NTS_SIZEOF
%token<child> NTS_IF
%token<child> NTS_ELSE
%token<child> NTS_FOR
%token<child> NTS_WHILE
%token<child> NTS_DO
%token<child> NTS_RETURN
%token<child> NTS_BREAK
%token<child> NTS_CONTINUE
%token<child> NTS_GOTO
%token<child> NTS_LAMBDA

/* operators */
%token<child>	NTO_PLUS 		2101
%token<child>	NTO_MINUS 		2102
%token<child>	NTO_DIVIDE 		2103
%token<child>	NTO_MODULO		2104
%token<child>	NTO_PRE_INCREMENT 		2111
%token<child>	NTO_PRE_DECREMENT		2112
%token<child>	NTO_POST_INCREMENT 		2113
%token<child>	NTO_POST_DECREMENT		2114

%token<child>	NTO_OR		2201
%token<child>	NTO_AND		2202
%token<child>	NTO_NOT		2203
%token<child>	NTO_BITWISE_OR		2211
%token<child>	NTO_BITWISE_AND		2212
%token<child>	NTO_BITWISE_NOT		2213
%token<child>	NTO_BITWISE_XOR		2214
%token<child>	NTO_SHIFT_LEFT		2221
%token<child>	NTO_SHIFT_RIGHT		2222

%token<child>	NTO_EQUAL			2311
%token<child>	NTO_NOT_EQUAL		2312
%token<child>	NTO_LESS_THAN		2313
%token<child>	NTO_GREATER_THAN	2315
%token<child>	NTO_LESS_OR_EQUAL		2314
%token<child>	NTO_GREATER_OR_EQUAL	2316

/* (nonterminals) */
%type<child> program declarations declaration args_list block decls_and_statements statements statement expression assignment operation place constant


 
 /* asociativity and priority (precendence) */
 /* http://www.linuxsoft.cz/article.php?id_article=486 */
%left NTO_OR
%left NTO_AND
%left NTO_BITWISE_OR
%left NTO_BITWISE_XOR
%left NTO_BITWISE_AND
%left NTO_MINUS NTO_PLUS 
%left NTO_TIMES NTO_DIVIDE NTO_MODULO 
%left TT_INCREMENT TT_DECREMENT
%right TT_PLUS  TT_MINUS NTO_PRE_INCREMENT NTO_PRE_DECREMENT NTO_NOT NTO_BITWISE_NOT 




   

     
%%

/* basic program rules */
/*
program: 
	top_level_statements	{ 
			*root = $$ = $1;
			SYNTAXER_LOG("whole program: %p", $1);
		}
;

top_level_statements:
	method_decl { 
			//TODO
		}
	| variable_decl	{ 
			//TODO
		}
;

declaration:
	  NT_IDENTIFIER TT_ASSIGNMENT expression TT_SEMICOLON {
			*root = $$ = create_decl_of_var($1, $3);
			SYNTAXER_LOG("declaration of var %p := %p -> %p", $1, $3, $$);
		}
	| NT_IDENTIFIER TT_NORMAL_LEFT_BRA args_list TT_NORMAL_RIGHT_BRA block {
			ast_node_t* proc = create_procedure($3, $5);
			*root = $$ = create_decl_of_var($1, proc);
			SYNTAXER_LOG("declaration of proc %p ( %p ) { %p } -> %p", $1, $3, $5, $$);
		}
;

args_list:
	  NT_IDENTIFIER TT_COMMA args_list  {
	  		*root = $$ = create_identifiers($3, $1);
	  		SYNTAXER_LOG("args list %p, %p... -> %p", $1, $3, $$);
	  	}
	| NT_IDENTIFIER {
	  		*root = $$ = create_identifier($1);
  		}
;
   
block:
 	TT_BLOCK_LEFT_BRA statements TT_BLOCK_RIGHT_BRA {
 		*root = $$ = $2;
 	}
;


statements: 
	  statement statements {
			*root = $$ = create_statements($1, $2);
		}
	| %empty {
			*root = $$ = NULL;
		}
;
*/
statement:
	  TT_SEMICOLON				{	*root = $$ = NULL;	SYNTAXER_LOG("empty statement");	}
	| expression TT_SEMICOLON	{	*root = $$ = $1;	SYNTAXER_LOG("expresion statement %p ;", $1);	}
;
 
expression:   
  	 constant 		{ *root = $$ = $1;	}
	| place 		{ *root = $$ = $1;	}
	| operation 	{ *root = $$ = $1;	}
	| assignment 	{ *root = $$ = $1;	}
	| TT_NORMAL_LEFT_BRA expression TT_NORMAL_RIGHT_BRA 
				{ *root = $$ = $2;	SYNTAXER_LOG("( %p ) -> %p", $2, $$);	}
	 
;

assignment:
	place TT_ASSIGNMENT expression;

operation:
	  expression NTO_AND expression	{ *root = $$ = create_binary(NTO_AND, $1, $3);	SYNTAXER_LOG("and   of %p %p -> %p", $1, $3, $$);	}
	| expression NTO_OR expression	{ *root = $$ = create_binary(NTO_OR, $1, $3);	SYNTAXER_LOG("or    of %p %p -> %p", $1, $3, $$);	}
	| NTO_NOT expression 	{ *root = $$ = create_unary(NTO_NOT, $2);	SYNTAXER_LOG("not   of %p -> %p", $2, $$);	}
	;

constant:
	  NT_NUMBER			{ *root = $$ = create_number($1);			SYNTAXER_LOG("number %ld -> %p", $1, $$);	}
	| NT_STRING			{ *root = $$ = create_string($1);			SYNTAXER_LOG("string %s -> %p", $1, $$);	}
;

place:
	NT_IDENTIFIER		{ *root = $$ = create_identifier($1);		SYNTAXER_LOG("identifier %s -> %p", $1, $$);}
;

%%

#include "../src/syntaxer-impl.c"