#ifndef _AST_H_
#define _AST_H_

typedef struct ast_node_t ast_node_t;

#include "../gen/syntaxer.h"
#include "tokens.h"

typedef struct ast_node_t {
	TOKEN_TYPE_T type;
	YYSTYPE value;
	struct ast_node_t* next;
} ast_node_t;

/*****************************************************************************/
/* help nodes and lists methods */
struct ast_node_t* create_new_node(TOKEN_TYPE_T type);
struct ast_node_t* prepend(struct ast_node_t* item, struct ast_node_t* list);
long lenght_of(struct ast_node_t* node);
struct ast_node_t* duplicate(struct ast_node_t* node);


/* constructors of lists */
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


/*****************************************************************************/
/* atomics */
struct ast_node_t* create_number(long value);
struct ast_node_t* create_identifier(char* name);
struct ast_node_t* create_string(char* value);


/* the whole program and declarations */
struct ast_node_t* create_program(struct ast_node_t* decls);

struct ast_node_t* create_procedure(struct ast_node_t* name,
		struct ast_node_t* params, struct ast_node_t* body);
struct ast_node_t* create_array_of_size(long size);
struct ast_node_t* create_array_of_value(struct ast_node_t* arrexpr);

struct ast_node_t* create_variables_decl(struct ast_node_t* decls);
struct ast_node_t* create_declaration(struct ast_node_t* var,
		struct ast_node_t* value);

struct ast_node_t* create_decl_of_proc(struct ast_node_t* name, struct ast_node_t* proc);

/* syntactic constructs */
struct ast_node_t* create_assignment(struct ast_node_t* place,
		struct ast_node_t* expr);
struct ast_node_t* create_if(struct ast_node_t* cond, struct ast_node_t* pos);
struct ast_node_t* create_if_else(struct ast_node_t* cond,
		struct ast_node_t* pos, struct ast_node_t* neg);
struct ast_node_t* create_for(struct ast_node_t* init, struct ast_node_t* cond,
		struct ast_node_t* inc, struct ast_node_t* body);
struct ast_node_t* create_while(struct ast_node_t* cond,
		struct ast_node_t* body);
struct ast_node_t* create_do_else(struct ast_node_t* body,
		struct ast_node_t* cond);
struct ast_node_t* create_keyword(TOKEN_TYPE_T keyword);
struct ast_node_t* create_return(struct ast_node_t* retexpr);

/* containers */
struct ast_node_t* create_block(struct ast_node_t* statements);
struct ast_node_t* create_parameters(struct ast_node_t* params);
struct ast_node_t* create_expressions(struct ast_node_t* exprs);

/* some expressions */
struct ast_node_t* create_proccall(struct ast_node_t* proc,
		struct ast_node_t* args);
struct ast_node_t* create_assignment_with_op(TOKEN_TYPE_T op,
		struct ast_node_t* place, struct ast_node_t* expr);
struct ast_node_t* create_reference(struct ast_node_t* of);
struct ast_node_t* create_dereference(struct ast_node_t* of);
struct ast_node_t* create_indexof(struct ast_node_t* of,
		struct ast_node_t* index);
struct ast_node_t* create_sizeof(struct ast_node_t* of);
struct ast_node_t* create_ternar_op(struct ast_node_t* cond,
		struct ast_node_t* pos, struct ast_node_t* neg);

/* operations */
struct ast_node_t* create_unary(TOKEN_TYPE_T operator, struct ast_node_t* expr);
struct ast_node_t* create_binary(TOKEN_TYPE_T operator,
		struct ast_node_t* expr1, struct ast_node_t* expr2);

#endif

