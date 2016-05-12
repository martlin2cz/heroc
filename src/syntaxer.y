%{

#include "../src/syntaxer-headers.h"

%}
/* settings */

%output "gen/syntaxer.c"
%defines "gen/syntaxer.h"

/*
 expression TT_PLUS expression	%prec NTO_PLUS		{ *root = $$ = create_binary(NTO_PLUS, $1, $3); 	SYNTAXER_LOG("plus	of %p %p -> %p", $1, $3, $$);	}
	|	expression TT_MINUS expression %prec NTO_MINUS	{ *root = $$ = create_binary(NTO_MINUS, $1, $3);	SYNTAXER_LOG("minus of %p %p -> %p", $1, $3, $$);	}
	|	expression TT_STAR expression	%prec NTO_TIMES		{ *root = $$ = create_binary(NTO_TIMES, $1, $3);	SYNTAXER_LOG("times of %p %p -> %p", $1, $3, $$);	}
	|	expression NTO_DIVIDE expression	{ *root = $$ = create_binary(NTO_DIVIDE, $1, $3);	SYNTAXER_LOG("div	 of %p %p -> %p", $1, $3, $$);	}
	|	expression NTO_MODULO expression	{ *root = $$ = create_binary(NTO_MODULO, $1, $3);	SYNTAXER_LOG("mod	 of %p %p -> %p", $1, $3, $$);	}
	
	|	TT_MINUS expression 		{ *root = $$ = create_unary(NTO_MINUS, $2);		SYNTAXER_LOG("unary minus of %p -> %p", $2, $$);	}
	|	TT_PLUS expression 		{ *root = $$ = create_unary(NTO_PLUS, $2);		SYNTAXER_LOG("unary plus of %p -> %p", $2, $$);	}
	
	|	TT_INCREMENT place %prec NTO_PRE_INCREMENT	{ *root = $$ = create_unary(NTO_PRE_INCREMENT, $2);	SYNTAXER_LOG("pre incrementation of %p -> %p", $2, $$);	}
	|	TT_DECREMENT place %prec NTO_PRE_DECREMENT	{ *root = $$ = create_unary(NTO_PRE_DECREMENT, $2);	SYNTAXER_LOG("pre decrementation of %p -> %p", $2, $$);	}
	|	place TT_INCREMENT 	{ *root = $$ = create_unary(NTO_POST_INCREMENT, $1);	SYNTAXER_LOG("post incrementation of %p -> %p", $1, $$);	}
	|	place TT_DECREMENT 	{ *root = $$ = create_unary(NTO_POST_DECREMENT, $1);	SYNTAXER_LOG("post decrementation of %p -> %p", $1, $$);	}
	*/

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
/* lexical tokens (not in use in ast) */
%token	TT_MINUS 				0001
%token	TT_PLUS 				0002
%token	TT_STAR 				0003
%token	TT_INCREMENT			0004
%token	TT_DECREMENT			0005
%token<oper>	TT_ASSIGNMENT			0011

/* special characters */
%token	TT_COMMA				0111
%token 	TT_SEMICOLON			0112
%token	TT_QUESTION_MARK		0113
%token	TT_COLON				0114
%token	TT_AMPERSAND				0115

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
%token<child> NTS_TYPEDEF		1101
%token<child> NTS_ASSIGNMENT	1102
%token<child> NTS_SIZEOF		1103
%token<child> NTS_IF			1104
%token<child> NTS_ELSE			1105
%token<child> NTS_FOR			1106
%token<child> NTS_WHILE			1107
%token<child> NTS_DO			1108
%token<child> NTS_RETURN		1109
%token NTS_BREAK			1110
%token NTS_CONTINUE		1111
%token NTS_GOTO			1112
%token<child> NTS_LAMBDA		1113

%token<child> NTS_ARRAY			1201

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
%type<child> program toplevel_toplevel_decl_statements toplevel_decl_statement procedure_decl variable_decl array_decl proc_params_list variables_decls_specs id_with_opt_asg_val
%type<child> block statements statement expressions expression 
%type<child> inblock_decl_statement  if_statement for_statement while_statement do_while_statement keyword 
%type<child> assignment operation funcall place constant array sizeof_expr


 
 /* asociativity and priority (precendence) */
 /* http://www.linuxsoft.cz/article.php?id_article=486 */
%right TT_ASSIGNMENT
%right TT_QUESTION_MARK
%left NTO_OR
%left NTO_AND
%left NTO_BITWISE_OR
%left NTO_BITWISE_XOR
%left NTO_BITWISE_AND
%left NTO_EQUAL NTO_NOT_EQUAL
%left NTO_LESS_THAN NTO_GREATER_THAN NTO_LESS_OR_EQUAL NTO_GRATER_OR_EQUAL
%left NTO_SHIFT_LEFT NTO_SHIFT_RIGHT
%left NTO_MINUS NTO_PLUS 
%left NTO_TIMES NTO_DIVIDE NTO_MODULO 
%left TT_INCREMENT TT_DECREMENT
%right TT_PLUS TT_MINUS NTO_PRE_INCREMENT NTO_PRE_DECREMENT NTO_NOT NTO_BITWISE_NOT 
%left TT_NORMAL_LEFT_BRA TT_INDEX_LEFT_BRA



%%

/* basic program rules */

program: 
	toplevel_toplevel_decl_statements { 
			*root = $$ = $1;
			SYNTAXER_LOG("whole program: %p", $$);
		}
;

toplevel_toplevel_decl_statements:
		%empty {
			*root = $$ = NULL;
		}
	|	toplevel_decl_statement toplevel_toplevel_decl_statements {
			*root = $$ = prepend($1, $2);
		}
;

toplevel_decl_statement:
		procedure_decl { 
			*root = $$ = $1;
		}
	|	variable_decl { 
			*root = $$ = $1;
		}
	|	array_decl {
			*root = $$ = $1;
		}
;

variable_decl:
		NTS_TYPEDEF variables_decls_specs TT_SEMICOLON {
			*root = $$ = $2;
			SYNTAXER_LOG("declaration of vars %p", $$);
		}
;

variables_decls_specs:
		id_with_opt_asg_val {
				*root = $$ = $1;
			}
	|	id_with_opt_asg_val TT_COMMA variables_decls_specs {
				*root = $$ = prepend($1, $3);
			}
;

id_with_opt_asg_val:
		NT_IDENTIFIER {
				ast_node_t* var = create_identifier($1);
				*root = $$ = create_decl_assignment(var, NULL);
				SYNTAXER_LOG("decl of variable %p -> %p", $1, $$);
			}
	|	NT_IDENTIFIER TT_ASSIGNMENT expression {
				ast_node_t* var = create_identifier($1);
				*root = $$ = create_decl_assignment(var, $3);
				SYNTAXER_LOG("decl of variable %p := %p -> %p", var, $3, $$);
			}
;

array_decl:
		NTS_TYPEDEF NT_IDENTIFIER TT_INDEX_LEFT_BRA NT_NUMBER TT_INDEX_RIGHT_BRA TT_SEMICOLON {
				ast_node_t* arr = create_array($4);
				*root = $$ = create_decl_assignment(arr, NULL);
				SYNTAXER_LOG("decl of array %p [%d] -> %p", arr, $4, $$);
		}
	|	NTS_TYPEDEF NT_IDENTIFIER TT_INDEX_LEFT_BRA NT_NUMBER TT_INDEX_RIGHT_BRA TT_ASSIGNMENT array TT_SEMICOLON {
				ast_node_t* arr = create_array($4);
				*root = $$ = create_decl_assignment(arr, $7);
				SYNTAXER_LOG("decl of variable %p [%d] := %p -> %p", arr, $4, $7, $$);
		}
;


procedure_decl:
	NT_IDENTIFIER TT_NORMAL_LEFT_BRA proc_params_list TT_NORMAL_RIGHT_BRA block {
			ast_node_t* id = create_identifier($1);			
			ast_node_t* proc = create_procedure($3, $5);
			*root = $$ = create_decl_assignment(id, proc);
			SYNTAXER_LOG("declaration of proc %p ( %p ) { %p } -> %p = %p -> %p", $1, $3, $5, id, proc, $$);
		}
;


proc_params_list:
		NT_IDENTIFIER TT_COMMA proc_params_list {
				ast_node_t* id = create_identifier($1);
				*root = $$ = prepend(id, $3);
				SYNTAXER_LOG("procedure parameter %p", id);
			}
	|	NT_IDENTIFIER {
				*root = $$ = create_identifier($1);
				SYNTAXER_LOG("procedure parameter %p", $1);
			}
;
	 
block:
	TT_BLOCK_LEFT_BRA statements TT_BLOCK_RIGHT_BRA {
		*root = $$ = create_block($2);
		SYNTAXER_LOG("block { %p } -> %p", $2, $$);
	}
;


statements: 
		statement statements {
			*root = $$ = prepend($1, $2);
		}
	|	%empty {
			*root = $$ = NULL;
		}
;

statement:
		TT_SEMICOLON						{	*root = $$ = NULL;	SYNTAXER_LOG("empty statement");	}
	|	expression TT_SEMICOLON	{	*root = $$ = $1;	SYNTAXER_LOG("statement with expression: %p", $1);	}
	|	inblock_decl_statement 			{ *root = $$ = $1;	SYNTAXER_LOG("declaration statement: %p", $1);	}
	|	if_statement 						{ *root = $$ = $1;	SYNTAXER_LOG("if: %p", $1);	}
	|	for_statement 					{ *root = $$ = $1;	SYNTAXER_LOG("for: %p", $1);	}
	|	while_statement 				{ *root = $$ = $1;	SYNTAXER_LOG("while: %p", $1);	}
	|	do_while_statement 			{ *root = $$ = $1;	SYNTAXER_LOG("do-while: %p", $1);	}
	|	keyword TT_SEMICOLON		{ *root = $$ = $1;	SYNTAXER_LOG("keyword: %p", $1);	}
	|	block				 						{ *root = $$ = $1;	SYNTAXER_LOG("inner block: %p", $1);	}
;

inblock_decl_statement:
		variable_decl { 
			*root = $$ = $1;
		}
	|	array_decl {
			*root = $$ = $1;
		}
;

if_statement:
		NTS_IF TT_NORMAL_LEFT_BRA expression TT_NORMAL_RIGHT_BRA block {
			*root = $$ = create_if($3, $5);
			SYNTAXER_LOG("if %p then { %p } -> %p", $3, $5, $$);
		}
	|	NTS_IF TT_NORMAL_LEFT_BRA expression TT_NORMAL_RIGHT_BRA block NTS_ELSE block {
			*root = $$ = create_if_else($3, $5, $7);
			SYNTAXER_LOG("if %p then { %p } else { %p }-> %p", $3, $5, $7, $$);
	}
;

for_statement:
	NTS_FOR TT_NORMAL_LEFT_BRA expression TT_SEMICOLON expression TT_SEMICOLON expression TT_NORMAL_RIGHT_BRA block {
			*root = $$ = create_for($3, $5, $7, $9);
			SYNTAXER_LOG("for ( %p ; %p ; %p ) do { %p } -> %p", $3, $5, $7, $9, $$);
	}
;
while_statement:
	NTS_WHILE TT_NORMAL_LEFT_BRA expression TT_NORMAL_RIGHT_BRA block {
				*root = $$ = create_while($3, $5);
		SYNTAXER_LOG("while ( %p ) do { %p } -> %p", $3, $5, $$);
	}
;	
do_while_statement:
	NTS_DO block NTS_WHILE TT_NORMAL_LEFT_BRA expression TT_NORMAL_RIGHT_BRA TT_SEMICOLON {
				*root = $$ = create_do_else($2, $5);
		SYNTAXER_LOG("do { %p } while ( %p ) -> %p", $2, $5, $$);
	}
;
keyword:
		NTS_CONTINUE {
			*root = $$ = create_keyword(NTS_CONTINUE);
			SYNTAXER_LOG("continue -> %p", $$);
		}
	|	NTS_BREAK {
			*root = $$ = create_keyword(NTS_BREAK);
			SYNTAXER_LOG("break -> %p", $$);
		}
	|	NTS_RETURN expression {
			*root = $$ = create_return($2);
			SYNTAXER_LOG("return %p -> %p", $2, $$);
	}
	|	NTS_GOTO {
		fprintf(stderr, "You shall not use goto!\n");
		$$ = NULL;
	}
;
 
expression:
		constant 	{ *root = $$ = $1;	}	/* TODO rly? */
	|	place 		{ *root = $$ = $1;	}	/* TODO rly? */
	|	operation 	{ *root = $$ = $1;	}
	|	assignment 	{ *root = $$ = $1;	}
	|	funcall		{ *root = $$ = $1;	}
	| 	sizeof_expr { *root = $$ = $1;	}
	|	TT_NORMAL_LEFT_BRA expression TT_NORMAL_RIGHT_BRA 
			{ *root = $$ = $2;	SYNTAXER_LOG("( %p ) -> %p", $2, $$);	}
	|	TT_AMPERSAND place 
			{ *root = $$ = create_reference($2);	SYNTAXER_LOG("reference & %p  -> %p", $2, $$);	}

;

funcall: 
	NT_IDENTIFIER TT_NORMAL_LEFT_BRA expressions TT_NORMAL_RIGHT_BRA {
		ast_node_t* id = create_identifier($1);
		*root = $$ = create_funcall(id, $3);
		SYNTAXER_LOG("funcall %p ( %p ) -> %p", id, $3, $$);
	}
;

expressions:
		%empty {
			$$ = NULL;
		}
	|	expression {
			*root = $$ = $1;
			SYNTAXER_LOG("expression in exprs: %p", $$);
		}
	|	expression TT_COMMA expressions {
			*root = $$ = prepend($1, $3);
			SYNTAXER_LOG("expression in exprs: %p, %p -> %p", $1, $3, $$);
		}
;
assignment:
	place TT_ASSIGNMENT expression {
		*root = $$ = create_assignment_with_op($2, $1, $3);
		SYNTAXER_LOG("assignment %p := %d %p -> %p", $1, $2, $3, $$);
	}
;

constant:
		NT_NUMBER			{ *root = $$ = create_number($1);			SYNTAXER_LOG("number %ld -> %p", $1, $$);	}
	|	NT_STRING			{ *root = $$ = create_string($1);			SYNTAXER_LOG("string %s -> %p", $1, $$);	}
;

sizeof_expr:
		NTS_SIZEOF TT_NORMAL_LEFT_BRA place TT_NORMAL_RIGHT_BRA { 
			*root = $$ = create_sizeof($3);	
			SYNTAXER_LOG("sizeof place %p -> %p", $3, $$);
		}
	|	NTS_SIZEOF TT_NORMAL_LEFT_BRA NT_IDENTIFIER TT_NORMAL_RIGHT_BRA { 
			ast_node_t* var = create_identifier($3);
			*root = $$ = create_sizeof(var);	
			SYNTAXER_LOG("sizeof variable %p -> %p", var, $$);
		}
;

place:
		NT_IDENTIFIER {
			*root = $$ = create_identifier($1);
			SYNTAXER_LOG("identifier %s -> %p", $1, $$);
		}
	|	NT_IDENTIFIER TT_INDEX_LEFT_BRA expression TT_INDEX_RIGHT_BRA {
			ast_node_t* id = create_identifier($1);
			*root = $$ = create_indexof(id, $3);
			SYNTAXER_LOG("indexing %p [ %p ] -> %p", id, $3, $$);
		}
	|	TT_STAR expression {
			*root = $$ = create_dereference($2);
			SYNTAXER_LOG("dereference * %p -> %p", $2, $$);
		}
;

array:
	TT_BLOCK_LEFT_BRA expressions TT_BLOCK_RIGHT_BRA {
		*root = $$ = $2;
	}
;

operation:
		expression TT_PLUS expression	{ BINARY_OP($1, $3, $$, NTO_PLUS)	}
	|	expression TT_MINUS expression	{ BINARY_OP($1,  $3, $$, NTO_MINUS)	}
	|	expression TT_STAR expression	{ BINARY_OP($1,  $3, $$, NTO_TIMES)	}
	|	expression NTO_DIVIDE expression	{ BINARY_OP($1,  $3, $$, NTO_DIVIDE)	}
	|	expression NTO_MODULO expression	{ BINARY_OP($1,  $3, $$, NTO_MODULO)	}
	|	expression NTO_OR expression		{ BINARY_OP($1,  $3, $$, NTO_OR)	}
	|	expression NTO_AND expression		{ BINARY_OP($1,  $3, $$, NTO_AND)	}
	|	expression NTO_BITWISE_OR expression	{ BINARY_OP($1,  $3, $$, NTO_BITWISE_OR)	}
	|	expression NTO_BITWISE_AND expression	{ BINARY_OP($1,  $3, $$, NTO_BITWISE_AND)	}
	|	expression NTO_BITWISE_XOR expression	{ BINARY_OP($1,  $3, $$, NTO_BITWISE_XOR)	}
	|	expression NTO_SHIFT_LEFT expression	{ BINARY_OP($1,  $3, $$, NTO_SHIFT_LEFT)	}
	|	expression NTO_SHIFT_RIGHT expression	{ BINARY_OP($1,  $3, $$, NTO_SHIFT_RIGHT)	}
	|	expression NTO_EQUAL expression			{ BINARY_OP($1,  $3, $$, NTO_EQUAL)	}
	|	expression NTO_NOT_EQUAL expression		{ BINARY_OP($1,  $3, $$, NTO_NOT_EQUAL)	}
	|	expression NTO_LESS_THAN expression		{ BINARY_OP($1,  $3, $$, NTO_LESS_THAN)	}
	|	expression NTO_GREATER_THAN expression	{ BINARY_OP($1,  $3, $$, NTO_GREATER_THAN)	}
	|	expression NTO_LESS_OR_EQUAL expression	{ BINARY_OP($1,  $3, $$, NTO_LESS_OR_EQUAL)	}
	|	expression NTO_GREATER_OR_EQUAL expression	{ BINARY_OP($1,  $3, $$, NTO_GREATER_OR_EQUAL)	}

	|	TT_PLUS expression 		{ UNARY_OP($2, $$, TT_PLUS)	}
	|	TT_MINUS expression 	{ UNARY_OP($2, $$, TT_MINUS)	}
	|	NTO_NOT expression 		{ UNARY_OP($2, $$, NTO_NOT)	}
	|	NTO_BITWISE_NOT expression 	{ UNARY_OP($2, $$, NTO_BITWISE_NOT)	}
	
	|	TT_INCREMENT place 	{ UNARY_OP($2, $$, NTO_PRE_INCREMENT)	}
	|	TT_DECREMENT place 	{ UNARY_OP($2, $$, NTO_PRE_INCREMENT)	}
	|	place TT_INCREMENT 	{ UNARY_OP($1, $$, NTO_POST_INCREMENT)	}
	|	place TT_DECREMENT  	{ UNARY_OP($1, $$, NTO_POST_INCREMENT)	}
	|	expression TT_QUESTION_MARK expression TT_COLON expression {
			*root = $$ = create_ternar_op($1, $3, $5);
			SYNTAXER_LOG("Ternary operator: %p ? %p : %p -> %p", $1, $3, $5, $$);
	}
;


%%

#include "../src/syntaxer-impl.c"
