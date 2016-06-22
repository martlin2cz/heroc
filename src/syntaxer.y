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

/* basic control characters */
%token JLT_COMMA			0101 	
%token JLT_SEMICOLON		0102
%token JLT_QUESTION_MARK	0103
%token JLT_COLON			0104
%token JLT_AMPERSAND		0105
%token<oper> JLT_ASSIGNMENT		0106

/* brackets */
%token JLT_NORMAL_LEFT_BRA	0201
%token JLT_NORMAL_RIGHT_BRA	0202
%token JLT_BLOCK_LEFT_BRA	0203
%token JLT_BLOCK_RIGHT_BRA	0204
%token JLT_INDEX_LEFT_BRA	0205
%token JLT_INDEX_RIGHT_BRA	0206

/* multi-used symbols */
%token JLT_MINUS			0301
%token JLT_PLUS				0302
%token JLT_STAR				0303
%token JLT_INCREMENT		0304
%token JLT_DECREMENT		0305

/* atomic values' tokens */
%token<number>	ATT_NUMBER		2101
%token<string>	ATT_STRING		2102
%token<string>	ATT_IDENTIFIER	2103

/* keywords and their particular control statements */
%token<child>	STK_ASSIGNMENT	2201
%token<child>	STK_TYPE		2202
%token<child>	STK_SIZEOF		2203
%token<child>	STK_IF			2204
%token<child>	STK_ELSE		2205
%token<child>	STK_FOR			2206
%token<child>	STK_WHILE		2207
%token<child>	STK_DO			2208
%token<child>	STK_RETURN		2209
%token<child>	STK_BREAK		2210
%token<child>	STK_CONTINUE	2211
%token<child>	STK_GOTO		2212	
%token<child>	STK_LAMBDA		2213

/* just semantic tokens */
%token<child>	JST_ARRAY			3301
%token<child>	JST_PROCEDURE		3302
%token<child>	JST_PROCCALL		3303
%token<child>	JST_VARIABLE_DECL	3304


/* containers */
%token<child>	CNT_STATEMENTS	3401
%token<child>	CNT_NUMBERS		3403
%token<child>	CNT_EXPRESSIONS	3404
%token<child>	CNT_PARAMETERS	3405	

/* meta nodes */
%token<child>	META_PREVIOUS	4001
%token<child>	META_LOOP		4002




/* operators/operations */
%token<child>	OPT_PLUS		1101
%token<child>	OPT_MINUS		1102
%token<child>	OPT_TIMES		1103
%token<child>	OPT_DIVIDE		1104
%token<child>	OPT_MODULO		1105
%token<child>	OPT_PRE_INCREMENT	1106
%token<child>	OPT_PRE_DECREMENT	1107
%token<child>	OPT_POST_INCREMENT	1108
%token<child>	OPT_POST_DECREMENT	1109

%token<child>	OPT_OR			1201
%token<child>	OPT_AND			1202
%token<child>	OPT_NOT			1203
%token<child>	OPT_BITWISE_OR	1204
%token<child>	OPT_BITWISE_AND	1205
%token<child>	OPT_BITWISE_NOT	1206
%token<child>	OPT_BITWISE_XOR	1207
%token<child>	OPT_SHIFT_LEFT	1208
%token<child>	OPT_SHIFT_RIGHT	1209

%token<child>	OPT_EQUAL			1301
%token<child>	OPT_NOT_EQUAL		1302
%token<child>	OPT_LESS_THAN		1303
%token<child>	OPT_GREATER_THAN	1304
%token<child>	OPT_LESS_OR_EQUAL	1305
%token<child>	OPT_GREATER_OR_EQUAL	1306

%token<child>	OPT_TERNARY		1401
%token<child>	OPT_REFERENCE 	1402
%token<child>	OPT_DEREFERENCE	1403
%token<child>	OPT_INDEX		1405

/* (nonterminals) */
%type<child> program toplevel_decl_statements toplevel_decl_statement
%type<child> procedure_decl variables_decl proc_params_list variables_decls_specs var_with_opt_asg_val
%type<child> block statements statement expressions expression emptyable_expression 
%type<child> inblock_decl_statement  if_statement for_statement while_statement do_while_statement keyword 
%type<child> assignment operation proccall place constant array sizeof_expr


 
 /* asociativity and priority (precendence) */
 /* http://www.linuxsoft.cz/article.php?id_article=486 */
%right JLT_ASSIGNMENT
%right JLT_QUESTION_MARK
%left OPT_OR
%left OPT_AND
%left OPT_BITWISE_OR
%left OPT_BITWISE_XOR
%left OPT_BITWISE_AND
%left OPT_EQUAL OPT_NOT_EQUAL
%left OPT_LESS_THAN OPT_GREATER_THAN OPT_LESS_OR_EQUAL OPT_GRATER_OR_EQUAL
%left OPT_SHIFT_LEFT OPT_SHIFT_RIGHT
%left JLT_MINUS JLT_PLUS 
%left OPT_TIMES OPT_DIVIDE OPT_MODULO 
%left JLT_INCREMENT JLT_DECREMENT
%right OPT_PLUS OPT_MINUS OPT_PRE_INCREMENT OPT_PRE_DECREMENT OPT_NOT OPT_BITWISE_NOT 
%left JLT_NORMAL_LEFT_BRA JLT_INDEX_LEFT_BRA



%%

/* basic program rules */

program: 
	toplevel_decl_statements { 
			*root = $$ = create_program($1);
			SYNTAXER_LOG("whole program: %p -> %p", $1, $$);
		}
;

toplevel_decl_statements:
		%empty {
			*root = $$ = NULL;
		}
	|	toplevel_decl_statement toplevel_decl_statements { 
			*root = $$ = prepend($1, $2);
		}
;

toplevel_decl_statement:
		procedure_decl { 
			*root = $$ = $1;
		}
	|	variables_decl {  
			*root = $$ = $1;
		}
;

variables_decl:
		STK_TYPE variables_decls_specs JLT_SEMICOLON { 
		
			*root = $$ = $2;
			SYNTAXER_LOG("declaration of vars %p ...", $2, $$);
		}
;

variables_decls_specs:
		var_with_opt_asg_val { 
				*root = $$ = $1;
			}
	|	var_with_opt_asg_val JLT_COMMA variables_decls_specs { 
				*root = $$ = prepend($1, $3);
			}
;

var_with_opt_asg_val:
		ATT_IDENTIFIER { 
				ast_node_t* var = create_identifier($1);
				*root = $$ = create_declaration(var, NULL);
				SYNTAXER_LOG("declaration %p -> %p", $1, $$);
			}
	|	ATT_IDENTIFIER JLT_ASSIGNMENT expression { 
				ast_node_t* var = create_identifier($1);
				*root = $$ = create_declaration(var, $3);
				SYNTAXER_LOG("declaration %p := %p -> %p", var, $3, $$);
			}
	|	ATT_IDENTIFIER JLT_INDEX_LEFT_BRA ATT_NUMBER JLT_INDEX_RIGHT_BRA { 
				ast_node_t* var = create_identifier($1);
				ast_node_t* array = create_array_of_size($3);
				*root = $$ = create_declaration(var, array);
				SYNTAXER_LOG("decl of array %p [%d] -> %p", var, $3, $$);
			}
	|	ATT_IDENTIFIER JLT_INDEX_LEFT_BRA JLT_INDEX_RIGHT_BRA JLT_ASSIGNMENT array {
				ast_node_t* var = create_identifier($1);
				ast_node_t* array = create_array_of_value($5);
				*root = $$ = create_declaration(var, array);
				SYNTAXER_LOG("decl of array %p [] := %p -> %p", var, $5, $$);
			}
;


procedure_decl:
	ATT_IDENTIFIER JLT_NORMAL_LEFT_BRA proc_params_list JLT_NORMAL_RIGHT_BRA block {
			ast_node_t* id = create_identifier($1);
			
			ast_node_t* params = create_parameters($3);
			ast_node_t* body = $5;
			ast_node_t* proc = create_procedure(id, params, body);
			
			*root = $$ = create_decl_of_proc(id, proc);
			SYNTAXER_LOG("declaration of proc %p ( %p ) { %p } -> %p := %p -> %p", id, params, body, id, proc, $$);
		}
;


proc_params_list:
		ATT_IDENTIFIER JLT_COMMA proc_params_list {
				ast_node_t* id = create_identifier($1);
				ast_node_t* decl = create_declaration(id, NULL);
				*root = $$ = prepend(decl, $3);
				SYNTAXER_LOG("procedure parameter %p", id);
			}
	|	ATT_IDENTIFIER {
				ast_node_t* id = create_identifier($1);
				ast_node_t* decl = create_declaration(id, NULL);
				
				*root = $$ = decl;
				SYNTAXER_LOG("procedure parameter %p", $1);
			}
	|	%empty {
				*root = $$ = NULL;
			}
;
	 
block:
	JLT_BLOCK_LEFT_BRA statements JLT_BLOCK_RIGHT_BRA {
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
	JLT_SEMICOLON					{	*root = $$ = NULL;	SYNTAXER_LOG("empty statement");	}
	|	expression JLT_SEMICOLON		{	*root = $$ = $1;	SYNTAXER_LOG("statement with expression: %p", $1);	}
	|	inblock_decl_statement 			{	*root = $$ = $1;	SYNTAXER_LOG("declaration statement: %p", $1);	}
	|	if_statement 					{	*root = $$ = $1;	SYNTAXER_LOG("if: %p", $1);	}
	|	for_statement 					{	*root = $$ = $1;	SYNTAXER_LOG("for: %p", $1);	}
	|	while_statement 				{	*root = $$ = $1;	SYNTAXER_LOG("while: %p", $1);	}
	|	do_while_statement 				{	*root = $$ = $1;	SYNTAXER_LOG("do-while: %p", $1);	}
	|	keyword JLT_SEMICOLON			{	*root = $$ = $1;	SYNTAXER_LOG("keyword: %p", $1);	}
	|	block				 			{	*root = $$ = $1;	SYNTAXER_LOG("inner block: %p", $1);	}
;

inblock_decl_statement:
		variables_decl { 
			*root = $$ = $1;
		}
;

if_statement:
		STK_IF JLT_NORMAL_LEFT_BRA expression JLT_NORMAL_RIGHT_BRA block {
			*root = $$ = create_if($3, $5);
			SYNTAXER_LOG("if %p then { %p } -> %p", $3, $5, $$);
		}
	|	STK_IF JLT_NORMAL_LEFT_BRA expression JLT_NORMAL_RIGHT_BRA block STK_ELSE block {
			*root = $$ = create_if_else($3, $5, $7);
			SYNTAXER_LOG("if %p then { %p } else { %p }-> %p", $3, $5, $7, $$);
	}
;

for_statement:
	STK_FOR JLT_NORMAL_LEFT_BRA emptyable_expression JLT_SEMICOLON emptyable_expression JLT_SEMICOLON emptyable_expression JLT_NORMAL_RIGHT_BRA block {
			*root = $$ = create_for($3, $5, $7, $9);
			SYNTAXER_LOG("for ( %p ; %p ; %p ) do { %p } -> %p", $3, $5, $7, $9, $$);
	}
;
while_statement:
	STK_WHILE JLT_NORMAL_LEFT_BRA expression JLT_NORMAL_RIGHT_BRA block {
				*root = $$ = create_while($3, $5);
		SYNTAXER_LOG("while ( %p ) do { %p } -> %p", $3, $5, $$);
	}
;	
do_while_statement:
	STK_DO block STK_WHILE JLT_NORMAL_LEFT_BRA expression JLT_NORMAL_RIGHT_BRA JLT_SEMICOLON {
				*root = $$ = create_do_else($2, $5);
		SYNTAXER_LOG("do { %p } while ( %p ) -> %p", $2, $5, $$);
	}
;
keyword:
		STK_CONTINUE {
			*root = $$ = create_keyword(STK_CONTINUE);
			SYNTAXER_LOG("continue -> %p", $$);
		}
	|	STK_BREAK {
			*root = $$ = create_keyword(STK_BREAK);
			SYNTAXER_LOG("break -> %p", $$);
		}
	|	STK_RETURN emptyable_expression {
			*root = $$ = create_return($2);
			SYNTAXER_LOG("return %p -> %p", $2, $$);
	}
	|	STK_GOTO {
		fprintf(stderr, "You shall not use goto!\n");
		$$ = NULL;
	}
;

emptyable_expression:
		%empty 		{	$$ = create_number(1);	SYNTAXER_LOG("empty expression, assuming 1 -> %p", $$);	}
	|	expression	{	$$ = $1;	}
 ;
 
expression:
		constant 	{ *root = $$ = $1;	}	/* TODO rly? */
	| 	array 		{ *root = $$ = $1;	}	/* will work? */	
	|	place 		{ *root = $$ = $1;	}	
	|	operation 	{ *root = $$ = $1;	}
	|	assignment 	{ *root = $$ = $1;	}
	|	proccall		{ *root = $$ = $1;	}
	| 	sizeof_expr { *root = $$ = $1;	}
	|	JLT_NORMAL_LEFT_BRA expression JLT_NORMAL_RIGHT_BRA 
			{ *root = $$ = $2;	SYNTAXER_LOG("( %p ) -> %p", $2, $$);	}

;

proccall: 
	expression JLT_NORMAL_LEFT_BRA expressions JLT_NORMAL_RIGHT_BRA {
		ast_node_t* args = create_expressions($3);
		*root = $$ = create_proccall($1, args);
		SYNTAXER_LOG("proccall %p ( %p ) -> %p", $1, args, $$);
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
	|	expression JLT_COMMA expressions {
			*root = $$ = prepend($1, $3);
			SYNTAXER_LOG("expression in exprs: %p, %p -> %p", $1, $3, $$);
		}
;

assignment:
	place JLT_ASSIGNMENT expression {
		*root = $$ = create_assignment_with_op($2, $1, $3);
		SYNTAXER_LOG("assignment %p := %d %p -> %p", $1, $2, $3, $$);
	}
;

constant:
		ATT_NUMBER			{ *root = $$ = create_number($1);			SYNTAXER_LOG("number %ld -> %p", $1, $$);	}
	|	ATT_STRING			{ *root = $$ = create_string($1);			SYNTAXER_LOG("string %s -> %p", $1, $$);	}
;

sizeof_expr:
		STK_SIZEOF JLT_NORMAL_LEFT_BRA place JLT_NORMAL_RIGHT_BRA { 
			*root = $$ = create_sizeof($3);	
			SYNTAXER_LOG("sizeof place %p -> %p", $3, $$);
		}
	|	STK_SIZEOF JLT_NORMAL_LEFT_BRA STK_TYPE JLT_NORMAL_RIGHT_BRA { 
			ast_node_t* type = create_keyword(STK_TYPE);
			*root = $$ = create_sizeof(type);	
			SYNTAXER_LOG("sizeof type %p -> %p", type, $$);
		}
;

place:
		ATT_IDENTIFIER {
			*root = $$ = create_identifier($1);
			SYNTAXER_LOG("identifier %s -> %p", $1, $$);
		}
	|	expression JLT_INDEX_LEFT_BRA expression JLT_INDEX_RIGHT_BRA {
			*root = $$ = create_indexof($1, $3);
			SYNTAXER_LOG("indexing %p [ %p ] -> %p", $1, $3, $$);
		}
	|	JLT_STAR expression {
			*root = $$ = create_dereference($2);
			SYNTAXER_LOG("dereference * %p -> %p", $2, $$);
		}
;

array:
	JLT_BLOCK_LEFT_BRA expressions JLT_BLOCK_RIGHT_BRA {
		*root = $$ = create_expressions($2);
		SYNTAXER_LOG("array of exprs: %p -> %p", $2, $$);
	}
;

operation:
		expression JLT_PLUS expression	{ BINARY_OP($1, $3, $$, OPT_PLUS)	}
	|	expression JLT_MINUS expression	{ BINARY_OP($1,  $3, $$, OPT_MINUS)	}
	|	expression JLT_STAR expression	{ BINARY_OP($1,  $3, $$, OPT_TIMES)	}
	|	expression JLT_AMPERSAND expression	{ BINARY_OP($1,  $3, $$, OPT_BITWISE_AND)	}
	|	expression OPT_DIVIDE expression	{ BINARY_OP($1,  $3, $$, OPT_DIVIDE)	}
	|	expression OPT_MODULO expression	{ BINARY_OP($1,  $3, $$, OPT_MODULO)	}
	|	expression OPT_OR expression		{ BINARY_OP($1,  $3, $$, OPT_OR)	}
	|	expression OPT_AND expression		{ BINARY_OP($1,  $3, $$, OPT_AND)	}
	|	expression OPT_BITWISE_OR expression	{ BINARY_OP($1,  $3, $$, OPT_BITWISE_OR)	}
	|	expression OPT_BITWISE_XOR expression	{ BINARY_OP($1,  $3, $$, OPT_BITWISE_XOR)	}
	|	expression OPT_SHIFT_LEFT expression	{ BINARY_OP($1,  $3, $$, OPT_SHIFT_LEFT)	}
	|	expression OPT_SHIFT_RIGHT expression	{ BINARY_OP($1,  $3, $$, OPT_SHIFT_RIGHT)	}
	|	expression OPT_EQUAL expression			{ BINARY_OP($1,  $3, $$, OPT_EQUAL)	}
	|	expression OPT_NOT_EQUAL expression		{ BINARY_OP($1,  $3, $$, OPT_NOT_EQUAL)	}
	|	expression OPT_LESS_THAN expression		{ BINARY_OP($1,  $3, $$, OPT_LESS_THAN)	}
	|	expression OPT_GREATER_THAN expression	{ BINARY_OP($1,  $3, $$, OPT_GREATER_THAN)	}
	|	expression OPT_LESS_OR_EQUAL expression	{ BINARY_OP($1,  $3, $$, OPT_LESS_OR_EQUAL)	}
	|	expression OPT_GREATER_OR_EQUAL expression	{ BINARY_OP($1,  $3, $$, OPT_GREATER_OR_EQUAL)	}

	|	JLT_PLUS expression 		{ UNARY_OP($2, $$, OPT_PLUS)	}
	|	JLT_MINUS expression 	{ UNARY_OP($2, $$, OPT_MINUS)	}
	|	OPT_NOT expression 		{ UNARY_OP($2, $$, OPT_NOT)	}
	|	OPT_BITWISE_NOT expression 	{ UNARY_OP($2, $$, OPT_BITWISE_NOT)	}
	
	|	JLT_INCREMENT place %prec OPT_PRE_INCREMENT	{ UNARY_OP($2, $$, OPT_PRE_INCREMENT)	}
	|	JLT_DECREMENT place	%prec OPT_PRE_DECREMENT	{ UNARY_OP($2, $$, OPT_PRE_DECREMENT)	}
	|	place JLT_INCREMENT 	{ UNARY_OP($1, $$, OPT_POST_INCREMENT)	}
	|	place JLT_DECREMENT  	{ UNARY_OP($1, $$, OPT_POST_DECREMENT)	}
	
	|	JLT_AMPERSAND place { 
			*root = $$ = create_reference($2);
			SYNTAXER_LOG("reference & %p  -> %p", $2, $$);	
		}
	|	expression JLT_QUESTION_MARK expression JLT_COLON expression {
			*root = $$ = create_ternar_op($1, $3, $5);
			SYNTAXER_LOG("Ternary operator: %p ? %p : %p -> %p", $1, $3, $5, $$);
		}
;


%%

#include "../src/syntaxer-impl.c"
