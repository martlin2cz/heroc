#ifndef _AST_C_
#define _AST_C_

#include <stdlib.h>
#include <stdio.h>

#include "../gen/syntaxer.h"
#include "ast.h"

/*****************************************************************************/
/* atoms */

struct ast_node_t* create_number(long value) {
	struct ast_node_t* node = create_new_node(ATT_NUMBER);
	node->value.number = value;
	return node;
}

struct ast_node_t* create_string(char* value) {
	struct ast_node_t* node = create_new_node(ATT_STRING);
	node->value.string = value;
	return node;
}

/*****************************************************************************/
/* the whole program and declarations */
struct ast_node_t* create_program(struct ast_node_t* decls) {
	return decls;
}

struct ast_node_t* create_identifier(char* name) {
	struct ast_node_t* name_node = create_new_node(ATT_STRING);
	name_node->value.string = name;

	return create_with_1_children(JST_VARIABLE, name_node);
}

struct ast_node_t* create_procedure(struct ast_node_t* name,
		struct ast_node_t* params, struct ast_node_t* body) {

	if (name == NULL) {
		ast_node_t* name = create_keyword(STK_LAMBDA); //TODO FIXME won't work, no? not tested yet! will create new var and recycle it
	} else {
		name = duplicate(name);
	}
	return create_with_3_children(JST_PROCEDURE, name, params, body);
}

struct ast_node_t* create_array_of_size(long size) {

	struct ast_node_t* s = create_number(size);
	return create_with_2_children(JST_ARRAY, s, NULL);
}
struct ast_node_t* create_array_of_value(struct ast_node_t* arrexpr) {
	long size = lenght_of(arrexpr->value.child);
	struct ast_node_t* s = create_number(size);
	return create_with_2_children(JST_ARRAY, s, arrexpr);
}

//struct ast_node_t* create_variables_decl(struct ast_node_t* decls) {
//	//TESTING return create_with_1_children(CNT_VARS_DECLS, decls);
//	return decls;
//}

struct ast_node_t* create_declaration(struct ast_node_t* var,
		struct ast_node_t* value) {

	return create_with_2_children(JST_VARIABLE_DECL, var, value);
}

struct ast_node_t* create_decl_of_proc(struct ast_node_t* var,
		struct ast_node_t* proc) {

	return create_declaration(var, proc);
}

/*****************************************************************************/
/* syntactic constructs */
struct ast_node_t* create_assignment(struct ast_node_t* place,
		struct ast_node_t* expr) {

	return create_with_2_children(STK_ASSIGNMENT, place, expr);
}

struct ast_node_t* create_if(struct ast_node_t* cond, struct ast_node_t* pos) {
	return create_with_2_children(STK_IF, cond, pos);
}

struct ast_node_t* create_if_else(struct ast_node_t* cond,
		struct ast_node_t* pos, struct ast_node_t* neg) {
	return create_with_3_children(STK_IF, cond, pos, neg);
}

struct ast_node_t* create_for(struct ast_node_t* init, struct ast_node_t* cond,
		struct ast_node_t* inc, struct ast_node_t* body) {
	return create_with_4_children(STK_FOR, init, cond, inc, body);
}

struct ast_node_t* create_while(struct ast_node_t* cond,
		struct ast_node_t* body) {
	return create_with_2_children(STK_WHILE, cond, body);
}

struct ast_node_t* create_do_else(struct ast_node_t* body,
		struct ast_node_t* cond) {
	return create_with_2_children(STK_DO, body, cond);
}

struct ast_node_t* create_keyword(TOKEN_TYPE_T keyword) {
	return create_with_0_children(keyword);
}

struct ast_node_t* create_return(struct ast_node_t* retexpr) {
	return create_with_1_children(STK_RETURN, retexpr);
}

/*****************************************************************************/
/* containers */
struct ast_node_t* create_block(struct ast_node_t* statements) {
	return create_with_1_children(CNT_STATEMENTS, statements);
}
struct ast_node_t* create_parameters(struct ast_node_t* params) {
	return create_with_1_children(CNT_PARAMETERS, params);
}
struct ast_node_t* create_expressions(struct ast_node_t* exprs) {
	return create_with_1_children(CNT_EXPRESSIONS, exprs);
}

/*****************************************************************************/
/* expressions */

struct ast_node_t* create_proccall(struct ast_node_t* proc,
		struct ast_node_t* args) {
	return create_with_2_children(JST_PROCCALL, proc, args);
}
struct ast_node_t* create_assignment_with_op(TOKEN_TYPE_T op,
		struct ast_node_t* place, struct ast_node_t* expr) {
	struct ast_node_t* value;

	if (op != JLT_ASSIGNMENT) {
		struct ast_node_t* place_copy = duplicate(place);
		value = create_binary(op, place_copy, expr);
	} else {
		value = expr;
	}

	return create_assignment(place, value);
}
struct ast_node_t* create_reference(struct ast_node_t* of) {
	return create_with_1_children(OPT_REFERENCE, of);
}
struct ast_node_t* create_dereference(struct ast_node_t* of) {
	return create_with_1_children(OPT_DEREFERENCE, of);
}
struct ast_node_t* create_indexof(struct ast_node_t* of,
		struct ast_node_t* index) {
	return create_with_2_children(OPT_INDEX, of, index);
}
struct ast_node_t* create_sizeof(struct ast_node_t* of) {
	return create_with_1_children(STK_SIZEOF, of);
}

struct ast_node_t* create_ternar_op(struct ast_node_t* cond,
		struct ast_node_t* pos, struct ast_node_t* neg) {
	return create_with_3_children(OPT_TERNARY, cond, pos, neg);
}

/*****************************************************************************/
/* operations */

struct ast_node_t* create_unary(TOKEN_TYPE_T operator, struct ast_node_t* expr) {
	return create_with_1_children(operator, expr);
}
struct ast_node_t* create_binary(TOKEN_TYPE_T operator,
		struct ast_node_t* expr1, struct ast_node_t* expr2) {
	return create_with_2_children(operator, expr1, expr2);
}

/*****************************************************************************/
/* lists */

struct ast_node_t* create_with_0_children(TOKEN_TYPE_T type) {
	struct ast_node_t* node = create_new_node(type);
	return node;
}

struct ast_node_t* create_with_1_children(TOKEN_TYPE_T type,
		struct ast_node_t* expr1) {
	struct ast_node_t* node = create_new_node(type);
	node->value.child = expr1;
	return node;
}
struct ast_node_t* create_with_2_children(TOKEN_TYPE_T type,
		struct ast_node_t* expr1, struct ast_node_t* expr2) {

	struct ast_node_t* node = create_new_node(type);
	node->value.child = expr1;
	expr1->next = expr2;
	return node;
}
struct ast_node_t* create_with_3_children(TOKEN_TYPE_T type,
		struct ast_node_t* expr1, struct ast_node_t* expr2,
		struct ast_node_t* expr3) {

	struct ast_node_t* node = create_new_node(type);
	node->value.child = expr1;
	expr1->next = expr2;
	expr2->next = expr3;
	return node;
}
struct ast_node_t* create_with_4_children(TOKEN_TYPE_T type,
		struct ast_node_t* expr1, struct ast_node_t* expr2,
		struct ast_node_t* expr3, struct ast_node_t* expr4) {

	struct ast_node_t* node = create_new_node(type);
	node->value.child = expr1;
	expr1->next = expr2;
	expr2->next = expr3;
	expr3->next = expr4;
	return node;
}

/*****************************************************************************/
/* help nodes and lists methods */

struct ast_node_t* create_new_node(TOKEN_TYPE_T type) {
	static int guid = 0;
	struct ast_node_t* node = (struct ast_node_t*) malloc(
			sizeof(struct ast_node_t));

	if (!node) {
		fprintf(stderr, "Cannot allocate memory");
		return NULL;
	}

	node->type = type;
	node->uid = guid;
	node->next = NULL;
	node->value.child = NULL;

	guid++;

	return node;
}

struct ast_node_t* prepend(struct ast_node_t* item, struct ast_node_t* list) {
	struct ast_node_t* curr_item = item;
	while (curr_item->next) {
		curr_item = curr_item->next;
	}

	curr_item->next = list;
	return item;
}

long lenght_of(struct ast_node_t* node) {
	long len = 0;
	while (node) {
		node = node->next;
		len++;
	}
	return len;
}

long lenght_ignore_meta(struct ast_node_t* node) {
	long len = 0;
	while (node) {
		if (!is_meta(node->type)) {
			len++;
		}
		node = node->next;
	}
	return len;
}

struct ast_node_t* duplicate(struct ast_node_t* node) {
	if (!node) {
		return NULL;
	}

	struct ast_node_t* copy = create_new_node(node->type);
	copy->next = duplicate(node->next);

	if (is_atomic(node->type)) {
		copy->value = node->value;
	} else {
		copy->value.child = duplicate(node->value.child);
	}

	return copy;
}

#endif
