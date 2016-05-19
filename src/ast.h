#ifndef _AST_H_
#define _AST_H_

typedef struct ast_node_t ast_node_t;

#include "../gen/syntaxer.h"
#include "tokens.h"

typedef struct ast_node_t {
	TOKEN_TYPE_T type;
	YYSTYPE value;
	//struct ast_node_t* next;
} ast_node_t;

typedef struct an_identifier_t {
	char* name;
} an_identifier_t;

typedef struct an_var_decl_t {
	an_identifier_t* identifier;
	ast_node_t* value;
} an_var_decl_t;

typedef struct an_array_t {
	int size;
	ast_node_t* items;
} an_array_t;

typedef struct an_procedure_t {
	an_identifier_t* name;
	ast_node_t* parameters;
	ast_node_t* body;
} an_procedure_t;

typedef struct an_operation_t {
	TOKEN_TYPE_T operator;
	ast_node_t* first_op;
	ast_node_t* second_op;
	ast_node_t* third_op;
} an_operation_t;

typedef struct an_assignment_t {
	ast_node_t* place;
	ast_node_t* value;
} an_assignment_t;

typedef struct an_proccall_t {
	ast_node_t* procedure;
	ast_node_t* arguments;
} an_proccall_t;

typedef struct an_if_stmt_t {
	ast_node_t* cond;
	ast_node_t* pos;
	ast_node_t* neg;
} an_if_stmt_t;

typedef struct an_for_stmt_t {
	ast_node_t* init;
	ast_node_t* cond;
	ast_node_t* incr;
	ast_node_t* body;
} an_for_stmt_t;

typedef struct an_while_stmt_t {
	ast_node_t* cond;
	ast_node_t* body;
} an_while_stmt_t;

typedef struct an_do_stmt_t {
	ast_node_t* cond;
	ast_node_t* body;
} an_do_stmt_t;

typedef struct an_return_stmt_t {
	ast_node_t* value;
} an_return_stmt_t;

typedef struct an_loop_cs_t {
	ast_node_t* loop;
} an_loop_cs_t;

typedef struct an_items_t {
	ast_node_t* value;
	struct an_items_t* next;
} an_items_t;

/*****************************************************************************/
/* help nodes and lists methods */
struct ast_node_t* create_new_node(TOKEN_TYPE_T type);

struct ast_node_t* duplicate(struct ast_node_t* node);

struct ast_node_t* nodify(TOKEN_TYPE_T type, void* value);


/* containers */
long lenght_of(struct an_items_t* node);
struct an_items_t* prepend(struct ast_node_t* item, struct an_items_t* list);

/*
 * XXX
 struct ast_node_t* create_with_0_children(TOKEN_TYPE_T type);
 struct ast_node_t* create_with_1_children(TOKEN_TYPE_T type,
 struct ast_node_t* expr1);
 struct ast_node_t* create_with_2_children(TOKEN_TYPE_T type,
 struct ast_node_t* expr1, struct ast_node_t* expr2);
 struct ast_node_t* create_with_3_children(TOKEN_TYPE_T type,
 struct ast_node_t* expr1, struct ast_node_t* expr2,
 struct ast_node_t* expr3);
 struct ast_node_t* create_with_4_children(TOKEN_TYPE_T type,
 struct ast_node_t* expr1, struct ast_node_t* expr2,
 struct ast_node_t* expr3, struct ast_node_t* expr4);
 */
/*****************************************************************************/
/* atomics */
struct ast_node_t* create_number(long value);
struct ast_node_t* create_identifier(char* name);
struct ast_node_t* create_string(char* value);

/* the whole program and declarations */
struct ast_node_t* create_program(struct an_items_t* decls);

struct an_procedure_t* create_procedure(struct an_identifier_t* name,
		struct an_items_t* params, struct ast_node_t* body);
struct an_array_t* create_array_of_size(long size);
struct an_array_t* create_array_of_value(struct ast_node_t* arrexpr);

struct an_items_t* create_variables_decl(struct an_items_t* decls);
struct an_var_decl_t* create_declaration(struct an_identifier_t* var,
		struct ast_node_t* value);

struct an_var_decl_t* create_decl_of_proc(struct an_identifier_t* name,
		struct an_procedure_t* proc);

/* syntactic constructs */
struct an_assignment_* create_assignment(struct ast_node_t* place,
		struct ast_node_t* expr);
struct an_if_stmt_t* create_if(struct ast_node_t* cond, struct ast_node_t* pos);
struct an_if_stmt_t* create_if_else(struct ast_node_t* cond,
		struct ast_node_t* pos, struct ast_node_t* neg);
struct an_for_stmt_t* create_for(struct ast_node_t* init, struct ast_node_t* cond,
		struct ast_node_t* inc, struct ast_node_t* body);
struct an_while_stmt_t* create_while(struct ast_node_t* cond,
		struct ast_node_t* body);
struct an_do_stmt_t* create_do_while(struct ast_node_t* body,
		struct ast_node_t* cond);
struct an_loop_cs_t* create_keyword(TOKEN_TYPE_T keyword);
struct an_return_stmt_t* create_return(struct ast_node_t* retexpr);

/* containers */
struct an_items_t* create_block(struct an_items_t* statements);
struct an_items_t* create_parameters(struct an_items_t* params);
struct an_items_t* create_expressions(struct an_items_t* exprs);

/* some expressions */
struct an_proccal_t* create_proccall(struct ast_node_t* proc,
		struct an_items_t* args);
struct an_assignment_t* create_assignment_with_op(TOKEN_TYPE_T op,
		struct ast_node_t* place, struct ast_node_t* expr);
struct an_operation_t* create_sizeof(struct ast_node_t* of);
struct an_operation_t* create_ternar_op(struct ast_node_t* cond,
		struct ast_node_t* pos, struct ast_node_t* neg);

/* operations */
struct an_operation_t* create_unary(TOKEN_TYPE_T operator, struct ast_node_t* expr);
struct an_operation_t* create_binary(TOKEN_TYPE_T operator,
		struct ast_node_t* expr1, struct ast_node_t* expr2);

#endif
