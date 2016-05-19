%{

#include "../src/syntaxer-headers.h"

%}
/* settings */

%output "gen/syntaxer.c"
%defines "gen/syntaxer.h"

/*
 expression_n TT_PLUS expression_n	%prec NTO_PLUS		{ *root = $$ = create_binary(NTO_PLUS, $1, $3); 	SYNTAXER_LOG("plus	of %p %p -> %p", $1, $3, $$);	}
	|	expression_n TT_MINUS expression_n %prec NTO_MINUS	{ *root = $$ = create_binary(NTO_MINUS, $1, $3);	SYNTAXER_LOG("minus of %p %p -> %p", $1, $3, $$);	}
	|	expression_n TT_STAR expression_n	%prec NTO_TIMES		{ *root = $$ = create_binary(NTO_TIMES, $1, $3);	SYNTAXER_LOG("times of %p %p -> %p", $1, $3, $$);	}
	|	expression_n NTO_DIVIDE expression_n	{ *root = $$ = create_binary(NTO_DIVIDE, $1, $3);	SYNTAXER_LOG("div	 of %p %p -> %p", $1, $3, $$);	}
	|	expression_n NTO_MODULO expression_n	{ *root = $$ = create_binary(NTO_MODULO, $1, $3);	SYNTAXER_LOG("mod	 of %p %p -> %p", $1, $3, $$);	}
	
	|	TT_MINUS expression_n 		{ *root = $$ = create_unary(NTO_MINUS, $2);		SYNTAXER_LOG("unary minus of %p -> %p", $2, $$);	}
	|	TT_PLUS expression_n 		{ *root = $$ = create_unary(NTO_PLUS, $2);		SYNTAXER_LOG("unary plus of %p -> %p", $2, $$);	}
	
	|	TT_INCREMENT place_n %prec NTO_PRE_INCREMENT	{ *root = $$ = create_unary(NTO_PRE_INCREMENT, $2);	SYNTAXER_LOG("pre incrementation of %p -> %p", $2, $$);	}
	|	TT_DECREMENT place_n %prec NTO_PRE_DECREMENT	{ *root = $$ = create_unary(NTO_PRE_DECREMENT, $2);	SYNTAXER_LOG("pre decrementation of %p -> %p", $2, $$);	}
	|	place_n TT_INCREMENT 	{ *root = $$ = create_unary(NTO_POST_INCREMENT, $1);	SYNTAXER_LOG("post incrementation of %p -> %p", $1, $$);	}
	|	place_n TT_DECREMENT 	{ *root = $$ = create_unary(NTO_POST_DECREMENT, $1);	SYNTAXER_LOG("post decrementation of %p -> %p", $1, $$);	}
	*/

%union {
	long number;
	char* string;
	int keyword_n;
	int oper;
	//XXX struct ast_node_t* child;
	struct an_identifier_t* identifier;
	struct an_var_decl_t* var_decl;
	struct an_array_v_t* array;
	struct an_procedure_t* procedure;
	struct an_operation_t* operation;
	struct an_assignment_v_t* assignment;
	struct an_proccall_t* proccall;
	
	struct an_if_stmt_t* if_stmt;
	struct an_for_stmt_t* for_stmt;
	struct an_while_stmt_t* while_stmt;
	struct an_do_stmt_t* do_stmt;
	struct an_return_stmt_t* return_stmt;
	struct an_loop_cs_t* loop_cs;
	
	struct an_items_t* items;
	
	struct ast_node_t* node;
	
}

%parse-param {struct ast_node_t** root}

%start program_n

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
%token<identifier>	ATT_IDENTIFIER	2103

/* keyword_ns and their particular control statements */

%token				STK_TYPE		2202
%token<operation>	STK_SIZEOF		2203
%token<if_stmnt>	STK_IF			2204
%token				STK_ELSE		2205
%token<for_stmt>	STK_FOR			2206
%token<while_stmt>	STK_WHILE		2207
%token<do_stmt>		STK_DO			2208
%token<return_stmt>	STK_RETURN		2209
%token<loop_cs>		STK_BREAK		2210
%token<loop_cs>		STK_CONTINUE	2211
%token				STK_GOTO		2212	
%token				STK_LAMBDA		2213
/* just semantic tokens */
%token<array>		JST_ARRAY			3301
%token<procedure>	JST_PROCEDURE		3302
%token<proccall>	JST_PROCCALL		3303
%token<operation>	JST_OPERATION		3304
%token<var_decl>	JST_VARIABLE_DECL	3305
%token<node>		JST_PROGRAM			2214
%token<assignment>	JST_ASSIGNMENT		2201


/* containers */
%token<items>	CNT_STATEMENTS	3401
%token<items>	CNT_VARS_DECLS	3402
%token<items>	CNT_NUMBERS		3403
%token<items>	CNT_EXPRESSIONS	3404
%token<items>	CNT_PARAMETERS	3405	


/* operators/operations */
%token<operation>	OPT_PLUS		1101
%token<operation>	OPT_MINUS		1102
%token<operation>	OPT_TIMES		1103
%token<operation>	OPT_DIVIDE		1104
%token<operation>	OPT_MODULO		1105
%token<operation>	OPT_PRE_INCREMENT	1106
%token<operation>	OPT_PRE_DECREMENT	1107
%token<operation>	OPT_POST_INCREMENT	1108
%token<operation>	OPT_POST_DECREMENT	1109

%token<operation>	OPT_OR			1201
%token<operation>	OPT_AND			1202
%token<operation>	OPT_NOT			1203
%token<operation>	OPT_BITWISE_OR	1204
%token<operation>	OPT_BITWISE_AND	1205
%token<operation>	OPT_BITWISE_NOT	1206
%token<operation>	OPT_BITWISE_XOR	1207
%token<operation>	OPT_SHIFT_LEFT	1208
%token<operation>	OPT_SHIFT_RIGHT	1209

%token<operation>	OPT_EQUAL			1301
%token<operation>	OPT_NOT_EQUAL		1302
%token<operation>	OPT_LESS_THAN		1303
%token<operation>	OPT_GREATER_THAN	1304
%token<operation>	OPT_LESS_OR_EQUAL	1305
%token<operation>	OPT_GREATER_OR_EQUAL	1306

%token<operation>	OPT_TERNARY		1401
%token<operation>	OPT_REFERENCE 	1402
%token<operation>	OPT_DEREFERENCE	1403
%token<operation>	OPT_INDEX		1405

/* (nonterminals) */
%type<node> program_n 
%type<items> toplevel_decl_statements_c  variables_decls_specs_c  
%type<node> variables_decl_n inblock_v_decl_statement_n toplevel_decl_statement_n  procedure_decl_n
%type<var_decl> var_with_opt_asg_val_v 
%type<items> block_v statements_c expressions_c  proc_params_list_c   
%type<node> statement_n expression_n emptyable_expression_n

%type<if_stmt>	if_statement_v
%type<for_stmt>	for_statement_v
%type<while_stmt>	while_statement_v
%type<do_stmt>	do_while_statement_v
%type<node>	keyword_n 
%type<assignment> assignment_v 
%type<operation>	operator_t 
%type<proccall>	proccall_v
%type<node>	place_n constant_n 
%type<array>	array_v
%type<operation>	sizeof_expr_v


 
 /* asociativity and priority (precendence) */
 /* http://www.linuxsoft.cz/article.php?id_article=486 */
%right JLT_assignment_v
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

/* basic program_n rules */

program_n: 
	toplevel_decl_statements_c { 
			struct an_items_t* decls = create_variables_decl($1);
			*root = $$ = nodify(JST_PROGRAM, create_program(decls));
			SYNTAXER_LOG("whole program_n: %p -> %p", $1, $$);
		}
;

toplevel_decl_statements_c:
		%empty {
			$$ = NULL;
		}
	|	toplevel_decl_statement_n toplevel_decl_statements_c { 
			$$ = prepend($1, $2);
		}
;

toplevel_decl_statement_n:
		procedure_decl_n { 
			*root = $$ = $1;
		}
	|	variables_decl_n {  
			*root = $$ = $1;
		}
;

variables_decl_n:
		STK_TYPE variables_decls_specs_c JLT_SEMICOLON { 
			$$ = nodify(CNT_VARS_DECLS, create_variables_decl($2));
			SYNTAXER_LOG("declaration of vars %p -> %p", $2, $$);
		}
;

variables_decls_specs_c:
		var_with_opt_asg_val_v { 
				$$ = prepend(nodify(JST_VARIABLE_DECL, $1), NULL);
			}
	|	var_with_opt_asg_val_v JLT_COMMA variables_decls_specs_c { 
				$$ = prepend(nodify(JST_VARIABLE_DECL, $1), $3);
			}
;

var_with_opt_asg_val_v:
		ATT_IDENTIFIER { 
				an_identifier_t* var = create_identifier($1);
				$$ = create_declaration(var, NULL);
				SYNTAXER_LOG("declaration %p -> %p", $1, $$);
			}
	|	ATT_IDENTIFIER JLT_ASSIGNMENT expression_n { 
				an_identifier_t* var = create_identifier($1);
				$$ = create_declaration(var, $3);
				SYNTAXER_LOG("declaration %p := %p -> %p", var, $3, $$);
			}
	|	ATT_IDENTIFIER JLT_INDEX_LEFT_BRA ATT_NUMBER JLT_INDEX_RIGHT_BRA { 
				an_identifier_t* var = create_identifier($1);
				an_array_t* array = create_array_of_size($3);
				$$ = create_declaration(var, array);
				SYNTAXER_LOG("decl of array_v %p [%d] -> %p", var, $3, $$);
			}
	|	ATT_IDENTIFIER JLT_INDEX_LEFT_BRA JLT_INDEX_RIGHT_BRA JLT_ASSIGNMENT array_v {
				an_identifier_t* var = create_identifier($1);
				an_array_t* array = create_array_of_value($5);
				$$ = create_declaration(var, array);
				SYNTAXER_LOG("decl of array_v %p [] := %p -> %p", var, $5, $$);
			}
;


procedure_decl_n:
	ATT_IDENTIFIER JLT_NORMAL_LEFT_BRA proc_params_list_c JLT_NORMAL_RIGHT_BRA block_v {
			ast_node_t* id = create_identifier($1);
			ast_node_t* params = create_parameters($3);
			ast_node_t* body = $5;
			
			//TODO nodify everywhere ...
			ast_node_t* proc = create_procedure(id, params, body);
			ast_node_t* decl = create_declaration(id, proc);
			ast_node_t* decls = prepend(decl, NULL);
			//$$ = nodify(CNT_VARS_DECLS, create_variables_decl(decls));	
			SYNTAXER_LOG("declaration of proc %p ( %p ) { %p } -> %p := %p -> %p", id, params, body, id, proc, $$);
		}
;


proc_params_list_c:
		ATT_IDENTIFIER JLT_COMMA proc_params_list_c {
				ast_node_t* id = create_identifier($1);
				*root = $$ = prepend(id, $3);
				SYNTAXER_LOG("procedure parameter %p", id);
			}
	|	ATT_IDENTIFIER {
				*root = $$ = create_identifier($1);
				SYNTAXER_LOG("procedure parameter %p", $1);
			}
	|	%empty {
				*root = $$ = NULL;
			}
;
	 
block_v:
	JLT_BLOCK_LEFT_BRA statements_c JLT_BLOCK_RIGHT_BRA {
		$$ = create_block($2);
		SYNTAXER_LOG("block { %p } -> %p", $2, $$);
	}
;


statements_c: 
		statement_n statements_c {
			*root = $$ = prepend($1, $2);
		}
	|	%empty {
			*root = $$ = NULL;
		}
;

statement_n:
	JLT_SEMICOLON						{	*root = $$ = NULL;	SYNTAXER_LOG("empty statement_n");	}
	|	expression_n JLT_SEMICOLON		{	*root = $$ = $1;	SYNTAXER_LOG("statement_n with expression_n: %p", $1);	}
	|	inblock_v_decl_statement_n 		{	*root = $$ = $1;	SYNTAXER_LOG("declaration statement_n: %p", $1);	}
	|	if_statement_v 					{	*root = $$ = nodify(STK_IF, $1);	SYNTAXER_LOG("if: %p", $1);	}
	|	for_statement_v 				{	*root = $$ = nodify(STK_FOR, $1);	SYNTAXER_LOG("for: %p", $1);	}
	|	while_statement_v 				{	*root = $$ = nodify(STK_WHILE, $1);	SYNTAXER_LOG("while: %p", $1);	}
	|	do_while_statement_v 			{	*root = $$ = nodify(STK_DO, $1);	SYNTAXER_LOG("do-while: %p", $1);	}
	|	keyword_n JLT_SEMICOLON			{	*root = $$ = $1;	SYNTAXER_LOG("keyword_n: %p", $1);	}
	|	block_v				 			{	*root = $$ = $1;	SYNTAXER_LOG("inner block_v: %p", $1);	}
;

inblock_v_decl_statement_n:
		variables_decl_n { 
			*root = $$ = $1;
		}
;

if_statement_v:
		STK_IF JLT_NORMAL_LEFT_BRA expression_n JLT_NORMAL_RIGHT_BRA block_v {
			$$ = create_if($3, $5);
			SYNTAXER_LOG("if %p then { %p } -> %p", $3, $5, $$);
		}
	|	STK_IF JLT_NORMAL_LEFT_BRA expression_n JLT_NORMAL_RIGHT_BRA block_v STK_ELSE block_v {
			$$ = create_if_else($3, $5, $7);
			SYNTAXER_LOG("if %p then { %p } else { %p }-> %p", $3, $5, $7, $$);
	}
;

for_statement_v:
	STK_FOR JLT_NORMAL_LEFT_BRA emptyable_expression_n JLT_SEMICOLON emptyable_expression_n JLT_SEMICOLON emptyable_expression_n JLT_NORMAL_RIGHT_BRA block_v {
		$$ = create_for($3, $5, $7, $9);
		SYNTAXER_LOG("for ( %p ; %p ; %p ) do { %p } -> %p", $3, $5, $7, $9, $$);
	}
;
while_statement_v:
	STK_WHILE JLT_NORMAL_LEFT_BRA expression_n JLT_NORMAL_RIGHT_BRA block_v {
		$$ = create_while($3, $5);
		SYNTAXER_LOG("while ( %p ) do { %p } -> %p", $3, $5, $$);
	}
;	
do_while_statement_v:
	STK_DO block_v STK_WHILE JLT_NORMAL_LEFT_BRA expression_n JLT_NORMAL_RIGHT_BRA JLT_SEMICOLON {
		$$ = create_do_while($2, $5);
		SYNTAXER_LOG("do { %p } while ( %p ) -> %p", $2, $5, $$);
	}
;
keyword_n:
		STK_CONTINUE {
			*root = $$ = nodify(STK_CONTINUE, create_keyword(STK_CONTINUE));
			SYNTAXER_LOG("continue -> %p", $$);
		}
	|	STK_BREAK {
			*root = $$ = nodify(STK_BREAK, create_keyword(STK_BREAK));
			SYNTAXER_LOG("break -> %p", $$);
		}
	|	STK_RETURN emptyable_expression_n {
			*root = $$ = nodify(STK_RETURN, create_return($2));
			SYNTAXER_LOG("return %p -> %p", $2, $$);
	}
	|	STK_GOTO {
		fprintf(stderr, "You shall not use goto!\n");
		$$ = NULL;
	}
;

emptyable_expression_n:
		%empty 		{	$$ = create_number(1);	SYNTAXER_LOG("empty expression_n, assuming 1 -> %p", $$);	}
	|	expression_n	{	$$ = $1;	}
 ;
 
expression_n:
		constant_n		{ *root = $$ = $1;	}	/* TODO rly? */
	|	place_n			{ *root = $$ = $1;	}
	|	operator_t		{ *root = $$ = nodify(JST_OPERATION, $1);	}
	| 	array_v 		{ *root = $$ = nodify(JST_ARRAY, $1);	}		
	|	assignment_v	{ *root = $$ = nodify(JST_ASSIGNMENT, $1);	}
	|	proccall_v		{ *root = $$ = nodify(JST_PROCCALL, $1);	}
	| 	sizeof_expr_v	{ *root = $$ = nodify(STK_SIZEOF, $1);	}
	|	JLT_NORMAL_LEFT_BRA expression_n JLT_NORMAL_RIGHT_BRA 
			{ *root = $$ = $2;	SYNTAXER_LOG("( %p ) -> %p", $2, $$);	}
;

expressions_c:
		%empty {
			$$ = NULL;
		}
	|	expression_n {
			$$ = $1;
			SYNTAXER_LOG("expression_n in exprs: %p", $$);
		}
	|	expression_n JLT_COMMA expressions_c {
			$$ = prepend($1, $3);
			SYNTAXER_LOG("expression_n in exprs: %p, %p -> %p", $1, $3, $$);
		}
;

proccall_v: 
	expression_n JLT_NORMAL_LEFT_BRA expressions_c JLT_NORMAL_RIGHT_BRA {
		ast_node_t* args = create_expressions($3);
		*root = $$ = create_proccall($1, args);
		SYNTAXER_LOG("proccall_v %p ( %p ) -> %p", $1, args, $$);
	}
;

assignment_v:
	place_n JLT_ASSIGNMENT expression_n {
		*root = $$ = create_assignment_with_op($2, $1, $3);
		SYNTAXER_LOG("assignment_v %p := %d %p -> %p", $1, $2, $3, $$);
	}
;

constant_n:
		ATT_NUMBER			{ *root = $$ = nodify(ATT_NUMBER, create_number($1));			SYNTAXER_LOG("number %ld -> %p", $1, $$);	}
	|	ATT_STRING			{ *root = $$ = nodify(ATT_STRING, create_string($1));			SYNTAXER_LOG("string %s -> %p", $1, $$);	}
;

sizeof_expr_v:
		STK_SIZEOF JLT_NORMAL_LEFT_BRA place_n JLT_NORMAL_RIGHT_BRA { 
			$$ = create_sizeof($3);	
			SYNTAXER_LOG("sizeof place_n %p -> %p", $3, $$);
		}
	|	STK_SIZEOF JLT_NORMAL_LEFT_BRA STK_TYPE JLT_NORMAL_RIGHT_BRA { 
			ast_node_t* type = create_keyword(STK_TYPE);
			$$ = create_sizeof(type);	
			SYNTAXER_LOG("sizeof type %p -> %p", type, $$);
		}
;

place_n:
		ATT_IDENTIFIER {
			*root = $$ = nodify(ATT_IDENTIFIER, create_identifier($1));
			SYNTAXER_LOG("identifier %s -> %p", $1, $$);
		}
	|	expression_n JLT_INDEX_LEFT_BRA expression_n JLT_INDEX_RIGHT_BRA {
			*root = $$ = nodify(JST_OPERATION, create_binary(OPT_INDEX, $1, $3));
			SYNTAXER_LOG("indexing %p [ %p ] -> %p", $1, $3, $$);
		}
	|	JLT_STAR expression_n {
			*root = $$ = nodify(JST_OPERATION, create_unary(OPT_DEREFERENCE, $2));
			SYNTAXER_LOG("dereference * %p -> %p", $2, $$);
		}
;

array_v:
	JLT_BLOCK_LEFT_BRA expressions_c JLT_BLOCK_RIGHT_BRA {
		$$ = create_expressions($2);
		SYNTAXER_LOG("array_v of exprs: %p -> %p", $2, $$);
	}
;

operator_t:
		expression_n JLT_PLUS expression_n	{ BINARY_OP($1, $3, $$, OPT_PLUS)	}
	|	expression_n JLT_MINUS expression_n	{ BINARY_OP($1,  $3, $$, OPT_MINUS)	}
	|	expression_n JLT_STAR expression_n	{ BINARY_OP($1,  $3, $$, OPT_TIMES)	}
	|	expression_n JLT_AMPERSAND expression_n	{ BINARY_OP($1,  $3, $$, OPT_BITWISE_AND)	}
	|	expression_n OPT_DIVIDE expression_n	{ BINARY_OP($1,  $3, $$, OPT_DIVIDE)	}
	|	expression_n OPT_MODULO expression_n	{ BINARY_OP($1,  $3, $$, OPT_MODULO)	}
	|	expression_n OPT_OR expression_n		{ BINARY_OP($1,  $3, $$, OPT_OR)	}
	|	expression_n OPT_AND expression_n		{ BINARY_OP($1,  $3, $$, OPT_AND)	}
	|	expression_n OPT_BITWISE_OR expression_n	{ BINARY_OP($1,  $3, $$, OPT_BITWISE_OR)	}
	|	expression_n OPT_BITWISE_XOR expression_n	{ BINARY_OP($1,  $3, $$, OPT_BITWISE_XOR)	}
	|	expression_n OPT_SHIFT_LEFT expression_n	{ BINARY_OP($1,  $3, $$, OPT_SHIFT_LEFT)	}
	|	expression_n OPT_SHIFT_RIGHT expression_n	{ BINARY_OP($1,  $3, $$, OPT_SHIFT_RIGHT)	}
	|	expression_n OPT_EQUAL expression_n			{ BINARY_OP($1,  $3, $$, OPT_EQUAL)	}
	|	expression_n OPT_NOT_EQUAL expression_n		{ BINARY_OP($1,  $3, $$, OPT_NOT_EQUAL)	}
	|	expression_n OPT_LESS_THAN expression_n		{ BINARY_OP($1,  $3, $$, OPT_LESS_THAN)	}
	|	expression_n OPT_GREATER_THAN expression_n	{ BINARY_OP($1,  $3, $$, OPT_GREATER_THAN)	}
	|	expression_n OPT_LESS_OR_EQUAL expression_n	{ BINARY_OP($1,  $3, $$, OPT_LESS_OR_EQUAL)	}
	|	expression_n OPT_GREATER_OR_EQUAL expression_n	{ BINARY_OP($1,  $3, $$, OPT_GREATER_OR_EQUAL)	}

	|	JLT_PLUS expression_n 		{ UNARY_OP($2, $$, OPT_PLUS)	}
	|	JLT_MINUS expression_n 	{ UNARY_OP($2, $$, OPT_MINUS)	}
	|	OPT_NOT expression_n 		{ UNARY_OP($2, $$, OPT_NOT)	}
	|	OPT_BITWISE_NOT expression_n 	{ UNARY_OP($2, $$, OPT_BITWISE_NOT)	}
	
	|	JLT_INCREMENT place_n %prec OPT_PRE_INCREMENT	{ UNARY_OP($2, $$, OPT_PRE_INCREMENT)	}
	|	JLT_DECREMENT place_n	%prec OPT_PRE_DECREMENT	{ UNARY_OP($2, $$, OPT_PRE_DECREMENT)	}
	|	place_n JLT_INCREMENT 	{ UNARY_OP($1, $$, OPT_POST_INCREMENT)	}
	|	place_n JLT_DECREMENT  	{ UNARY_OP($1, $$, OPT_POST_DECREMENT)	}
	
	|	JLT_AMPERSAND place_n 	{ UNARY_OP($2, $$, OPT_REFERENCE)	}
	|	expression_n JLT_QUESTION_MARK expression_n JLT_COLON expression_n {
			$$ = create_ternar_op($1, $3, $5);
			SYNTAXER_LOG("Ternary operator: %p ? %p : %p -> %p", $1, $3, $5, $$);
		}
;


%%

#include "../src/syntaxer-impl.c"
