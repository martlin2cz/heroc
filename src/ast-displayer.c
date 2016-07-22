#ifndef _AST_DISPLAYER_C_
#define _AST_DISPLAYER_C_

#include <stdio.h>

#include <unistd.h>
#include "misc.h"
#include "ast.h"
#include "tokens.h"
#include "ast-displayer.h"

void ast_display_root(FILE* dest, struct ast_node_t* node) {
	ast_display_nodes(dest, node, 0);
}

void ast_display_nodes(FILE* dest, struct ast_node_t* nodes, int padding) {
	ast_node_t* node = nodes;

	while (node) {
		ast_display_single_node(dest, node, padding);

		node = node->next;
	}
}

void ast_display_single_node(FILE* dest, struct ast_node_t* node, int padding) {
	//printf("[NODE:%p \t type:%4d \t next:%9p \t value: %9x]\n", node, node->type, node->next, node->value.child);

	TOKEN_TYPE_T type = node->type;
	if (is_atomic(type)) {
		ast_display_atomic(dest, node, padding);

	} else if (is_meta(type)) {
		ast_display_meta(dest, node, padding);

	} else {
		ast_display_compozite(dest, node, padding);
	}

}

void ast_display_atomic(FILE* dest, struct ast_node_t* node, int padding) {
	print_padding(dest, padding);

	switch (node->type) {
	case ATT_NUMBER:
		fprintf(dest, "number: %ld \n", node->value.number);
		return;

	case ATT_STRING:
		fprintf(dest, "string: %s\n", node->value.string);
		return;
	default:
		fprintf(stderr, "da: Unknown type of type to export: %d", node->type);
		return;
	}
}

void ast_display_meta(FILE* dest, struct ast_node_t* node, int padding) {
	print_padding(dest, padding);
	fprintf(dest, "[Meta] %s", to_string(node->type));

	switch (node->type) {
	case META_ARITY_OF_EXTERNAL:
			fprintf(dest, " of arity %d ", node->value.number);
			break;
	case META_ADRESS:
		fprintf(dest, " at %+d ", node->value.number);
		break;
	case META_VAR_TYPE:
		fprintf(dest, " of type %d ", node->value.number);
		break;
	case META_DECLARATION:
	case META_LOOP:
	case META_PREVIOUS:
		if (node->value.child) {
			fprintf(dest, " at %p (%d)", node->value.child,
					node->value.child->uid);
		} else {
			fprintf(dest, " at NULL");
		}
		break;
	default:
		fprintf(dest, "\n");
		fprintf(stderr, "dm: Unknown type of type to export: %d", node->type);
		return;
	}

	fprintf(dest, "\n");
}

void ast_display_compozite(FILE* dest, struct ast_node_t* node, int padding) {
	print_padding(dest, padding);
	fprintf(dest, "Node %s %p (%d)\n", to_string(node->type), node, node->uid);
	ast_display_nodes(dest, node->value.child, padding + 1);
}

#endif
