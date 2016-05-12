#ifndef _AST_BASIC_EXPORTER_C_
#define _AST_BASIC_EXPORTER_C_

#include <stdio.h>
#include "misc.h"
#include "ast.h"
#include "tokens.h"
#include "ast-basic-exporter.h"

void ast_export_root(FILE* dest, struct ast_node_t* node) {


	/*
	print_node(dest, node, 0);
	*/
	fprintf(dest, "[FIXME, cycling!]\n");	//TODO FIXME
}

void print_node(FILE* dest, struct ast_node_t* node, int padding) {
	//printf("[NODE:%p \t type:%4d \t next:%9p \t value: %9x]\n", node, node->type, node->next, node->value.child);
	while (node != NULL) {

		TOKEN_TYPE_T type = node->type;
		if (is_atomic(type)) {
			print_atomic(dest, node, padding);
		} else {
			print_compozite(dest, node, padding);
		}

		node = node->next;
	}
}

void print_atomic(FILE* dest, struct ast_node_t* node, int padding) {
	print_padding(dest, padding);

	switch (node->type) {
	case NT_NUMBER:
		fprintf(dest, "Number: %ld \n", node->value.number);
		return;
	case NT_IDENTIFIER:
		fprintf(dest, "Identifier: %s\n", node->value.string);
		return;
	case NT_STRING:
		fprintf(dest, "String: %s\n", node->value.string);
		return;
	default:
		fprintf(stderr, "unknown type of type to export: %d", node->type);
		return;
	}
}

void print_compozite(FILE* dest, struct ast_node_t* node, int padding) {
	print_padding(dest, padding);
	printf("Node %04d\n", node->type);

	print_node(dest, node->value.child, padding + 1);
}

/*
 void print_construct(FILE* dest, struct ast_node_t* node, int padding) {
 print_padding(dest, padding);
 fprintf(stderr, "TODO: construct %d", node->type);
 }

 void print_operator(FILE* dest, struct ast_node_t* node, int padding) {
 print_padding(dest, padding);
 char* strval = oper_to_str(node->type);
 fprintf(dest, "Operator %s on:\n", strval);
 if (node->value.child) {
 print_node(dest, node->value.child, padding + 1);
 }
 }
 */

#endif
