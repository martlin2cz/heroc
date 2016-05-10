#ifndef _AST_C_
#define _AST_C_

#include <stdlib.h>
#include <stdio.h>

#include "../gen/syntaxer.h"
#include "ast.h"



struct ast_node_t* create_new_node(TOKEN_TYPE_T type) {
	struct ast_node_t* node = (struct ast_node_t*) malloc(
			sizeof(struct ast_node_t));
	if (!node) {
		fprintf(stderr, "Cannot allocate memory");
		return NULL;
	}

	node->type = type;
	node->next = NULL;
	node->value.child = NULL;

	return node;
}

struct ast_node_t* create_number(long value) {
	struct ast_node_t* node = create_new_node(NT_NUMBER);
	node->value.number = value;
	return node;
}

struct ast_node_t* create_identifier(char* name) {
	struct ast_node_t* node = create_new_node(NT_IDENTIFIER);
	node->value.string = name;
	return node;
}

struct ast_node_t* create_string(char* value) {
	struct ast_node_t* node = create_new_node(NT_STRING);
	node->value.string = value;
	return node;
}


//TODO ...
struct ast_node_t*  create_declarations(struct ast_node_t* decls, struct ast_node_t* decl) {
	return NULL;
}
struct ast_node_t*  create_decl_of_var(struct ast_node_t* var, struct ast_node_t* value) {
	return NULL;
}
struct ast_node_t*  create_procedure(struct ast_node_t* args, struct ast_node_t* body) {
	return NULL;
}
struct ast_node_t*  create_identifiers(struct ast_node_t* ids, struct ast_node_t* id) {
	return NULL;
}
struct ast_node_t*  create_decls_and_statements(struct ast_node_t* decls, struct ast_node_t* statemens) {
	return NULL;
}
struct ast_node_t*  create_statements(struct ast_node_t* statement, struct ast_node_t* statements) {
	return NULL;
}




struct ast_node_t* create_unary(TOKEN_TYPE_T operator, struct ast_node_t* expr) {
	return create_list1(operator, expr);
}
struct ast_node_t* create_binary(TOKEN_TYPE_T operator,
		struct ast_node_t* expr1, struct ast_node_t* expr2) {
	return create_list2(operator, expr1, expr2);
}
struct ast_node_t* create_ternary(TOKEN_TYPE_T operator,
		struct ast_node_t* expr1, struct ast_node_t* expr2,
		struct ast_node_t* expr3) {
	return create_list3(operator, expr1, expr2, expr3);
}

struct ast_node_t* create_list1(TOKEN_TYPE_T type, struct ast_node_t* expr1) {
	struct ast_node_t* node = create_new_node(type);
	node->value.child = expr1;
	return node;
}
struct ast_node_t* create_list2(TOKEN_TYPE_T type, struct ast_node_t* expr1,
		struct ast_node_t* expr2) {

	struct ast_node_t* node = create_new_node(type);
	node->value.child = expr1;
	expr1->next = expr2;
	return node;
}
struct ast_node_t* create_list3(TOKEN_TYPE_T type, struct ast_node_t* expr1,
		struct ast_node_t* expr2, struct ast_node_t* expr3) {

	struct ast_node_t* node = create_new_node(type);
	node->value.child = expr1;
	expr1->next = expr2;
	expr2->next = expr3;
	return node;
}
struct ast_node_t* create_list4(TOKEN_TYPE_T type, struct ast_node_t* expr1,
		struct ast_node_t* expr2, struct ast_node_t* expr3,
		struct ast_node_t* expr4) {

	struct ast_node_t* node = create_new_node(type);
	node->value.child = expr1;
	expr1->value.child = expr2;
	expr2->value.child = expr3;
	expr3->value.child = expr4;
	return node;
}

#if 0
//TODO XXX FIXME commented out
/*
 Leaf nodes of syntax tree
 */

syntax_node_t* create_number(int number) {
	syntax_node_t* node = (syntax_node_t*) malloc(sizeof(syntax_node_t));
	if (!node)
	return NULL;

	node->type = NUMBER;
	node->value.number = number;
	node->next = NULL;
	node->child = NULL;

	return node;
}

syntax_node_t* create_text(char* text) {
	syntax_node_t* node = (syntax_node_t*) malloc(sizeof(syntax_node_t));
	if (!node)
	return NULL;

	node->type = TEXT;
	node->value.text = text;
	node->next = NULL;
	node->child = NULL;

	return node;
}

syntax_node_t* create_variable(char* text) {
	syntax_node_t* node = (syntax_node_t*) malloc(sizeof(syntax_node_t));
	if (!node)
	return NULL;

	node->type = VARIABLE;
	node->value.text = text;
	node->next = NULL;
	node->child = NULL;

	return node;
}

syntax_node_t* create_keyword(special_t keyword) {
	syntax_node_t* node = (syntax_node_t*) malloc(sizeof(syntax_node_t));
	if (!node)
	return NULL;

	node->type = keyword;
	node->next = NULL;
	node->child = NULL;

	return node;

}

/*
 Expressions
 */
syntax_node_t* create_assignment(operator_t asg_operator, syntax_node_t* variable, syntax_node_t* expr) {
	syntax_node_t* node = create_list2(ASSIGNMENT, variable, expr);
	if (!node)
	return NULL;

	node->value.operator = asg_operator;
	return node;
}

syntax_node_t* create_unary(operator_t operator, syntax_node_t* expr) {
	syntax_node_t* node = create_list1(OPERATION, expr);
	if (!node)
	return NULL;

	node->value.operator = operator;
	return node;
}

syntax_node_t* create_binary(operator_t operator, syntax_node_t* expr1, syntax_node_t* expr2) {
	syntax_node_t* node = create_list2(OPERATION, expr1, expr2);
	if (!node)
	return NULL;

	node->value.operator = operator;
	return node;
}

/*
 Statements
 */
syntax_node_t* create_print(syntax_node_t* string, syntax_node_t* expr) {
	syntax_node_t* node;

	if (!string)
	node = create_list1(SPECIAL, expr);
	else if (!expr)
	node = create_list1(SPECIAL, string);
	else
	node = create_list2(SPECIAL, string, expr);

	if (!node)
	return NULL;

	node->value.special = PRINT;
	return node;
}

syntax_node_t* create_scan(syntax_node_t* var) {
	syntax_node_t* node = create_list1(SPECIAL, var);

	if (!node)
	return NULL;

	node->value.special = SCAN;
	return node;
}

syntax_node_t* create_if(syntax_node_t* cond, syntax_node_t* cmd, syntax_node_t* cmdelse) {
	syntax_node_t* node;

	if (cmdelse)
	node = create_list3(SPECIAL, cond, cmd, cmdelse);
	else
	node = create_list2(SPECIAL, cond, cmd);

	if (!node)
	return NULL;

	node->value.special = IF;
	return node;
}

syntax_node_t* create_for(syntax_node_t* init, syntax_node_t* cond, syntax_node_t* incr, syntax_node_t* cmd) {
	syntax_node_t* node = create_list4(SPECIAL, init, cond, incr, cmd);

	if (!node)
	return NULL;

	node->value.special = FOR;
	return node;
}

syntax_node_t* create_do_while(syntax_node_t* cond, syntax_node_t* cmd) {
	syntax_node_t* node = create_list2(SPECIAL, cond, cmd);

	if (!node)
	return NULL;

	node->value.special = DO_WHILE;
	return node;
}
syntax_node_t* create_while(syntax_node_t* cond, syntax_node_t* cmd) {
	syntax_node_t* node = create_list2(SPECIAL, cond, cmd);

	if (!node)
	return NULL;

	node->value.special = WHILE;
	return node;
}

syntax_node_t* create_commands(syntax_node_t* command, syntax_node_t* commands) {
	syntax_node_t* node = create_list2(COMMANDS, command, commands);

	if (!node)
	return NULL;

	return node;
}

syntax_node_t* create_empty_command() {
	syntax_node_t* node = create_list1(COMMANDS, NULL);

	if (!node)
	return NULL;

	return node;
}

/*
 Creating children lists
 */
syntax_node_t* create_list1(syntax_node_type type, syntax_node_t* expr1) {
	syntax_node_t* node = (syntax_node_t*) malloc(sizeof(syntax_node_t));
	if (!node)
	return NULL;

	node->type = type;
	node->child = expr1;

	return node;
}

syntax_node_t* create_list2(syntax_node_type type, syntax_node_t* expr1, syntax_node_t* expr2) {
	syntax_node_t* node = (syntax_node_t*) malloc(sizeof(syntax_node_t));
	if (!node)
	return NULL;

	node->type = type;
	node->child = expr1;

	expr1->next = expr2;

	return node;
}

syntax_node_t* create_list3(syntax_node_type type, syntax_node_t* expr1, syntax_node_t* expr2, syntax_node_t* expr3) {
	syntax_node_t* node = (syntax_node_t*) malloc(sizeof(syntax_node_t));
	if (!node)
	return NULL;

	node->type = type;
	node->child = expr1;

	expr1->next = expr2;
	expr2->next = expr3;

	return node;

}

syntax_node_t* create_list4(syntax_node_type type, syntax_node_t* expr1, syntax_node_t* expr2, syntax_node_t* expr3, syntax_node_t* expr4) {
	syntax_node_t* node = (syntax_node_t*) malloc(sizeof(syntax_node_t));
	if (!node)
	return NULL;

	node->type = type;
	node->child = expr1;

	expr1->next = expr2;
	expr2->next = expr3;
	expr3->next = expr4;

	return node;

}

/*
 Printing
 */
void print_root(syntax_node_t* root) {
	print_node(0, root);
}

void print_padding(int padding) {
	int i;
	for (i = 0; i < padding; i++)
	printf("  ");
	printf("% 2d: ", padding);
}

void print_node(int padding, syntax_node_t* node) {

	if (padding >= 0)
	print_padding(padding);
	else
	padding = -padding;

	if (!node) {
		printf("<<NULL>>\n");
		return;
	}

	print_node_label(node);

	if (node->type == SPECIAL) {
		print_special(padding + 1, node);
	} else {
		syntax_node_t* child = node->child;
		while (child) {
			print_node(padding + 1, child);
			child = child->next;
		}
	}
}

void print_node_label(syntax_node_t* node) {
	switch (node->type) {
		case NUMBER:
		printf("NUMBER=%d\n", node->value.number);
		break;
		case TEXT:
		printf("TEXT=\"%s\"\n", node->value.text);
		break;
		case VARIABLE:
		printf("VARIABLE=%s\n", node->value.text);
		break;
		case OPERATION:
		printf("OPERATOR=%x\n", node->value.operator);
		break;
		case ASSIGNMENT:
		printf("ASG OPR=%x =\n", node->value.operator);
		break;
		case COMMANDS:
		printf("COMMANDS:\n");
		break;
		case SPECIAL:
		printf("SPECIAL: ");
		return;
	}
}

void print_special(int padding, syntax_node_t* node) {
	syntax_node_t* c;
	switch (node->value.special) {
		case IF:
		printf("IF\n");

		print_padding(padding);
		printf("if  ");
		c = node->child;
		print_node(-padding, c);

		print_padding(padding);
		printf("then  ");
		c = c->next;
		print_node(-padding, c);

		if (c->next) {
			print_padding(padding);
			printf("else  ");
			c = c->next;
			print_node(-padding, c);
		}
		break;

		case FOR:
		printf("FOR\n");

		print_padding(padding);
		printf("init  ");
		c = node->child;
		print_node(-padding, c);

		print_padding(padding);
		printf("cond  ");
		c = c->next;
		print_node(-padding, c);

		print_padding(padding);
		printf("incr  ");
		c = c->next;
		print_node(-padding, c);

		print_padding(padding);
		printf("cmds  ");
		c = c->next;
		print_node(-padding, c);

		break;
		case WHILE:
		printf("WHILE\n");

		print_padding(padding);
		printf("while  ");
		c = node->child;
		print_node(-padding, c);

		print_padding(padding);
		printf("do  ");
		c = c->next;
		print_node(-padding, c);

		break;
		case DO_WHILE:
		printf("DO-WHILE\n");

		print_padding(padding);
		printf("do  ");
		c = node->child;
		print_node(-padding, c);

		print_padding(padding);
		printf("while  ");
		c = c->next;
		print_node(-padding, c);

		break;
		case BREAK:
		printf("BREAK\n");
		break;
		case CONTINUE:
		printf("CONTINUE\n");
		break;
		case PRINT:
		printf("PRINT\n");
		c = node->child;

		if (c->type == TEXT) {
			print_padding(padding);
			printf("string  ");
			print_node(-padding, c);
			c = c->next;
		}

		if (c) {
			print_padding(padding);
			printf("value  ");
			print_node(-padding, c);
		}

		break;
		case SCAN:
		printf("SCAN\n");

		print_padding(padding);
		printf("scan into  ");
		c = node->child;
		print_node(-padding, c);
		break;
	}
}
#endif
#endif
