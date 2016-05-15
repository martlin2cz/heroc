#ifndef _AST_BASIC_EXPORTER_C_
#define _AST_BASIC_EXPORTER_C_

#include <stdio.h>

#include <unistd.h>
#include "misc.h"
#include "ast.h"
#include "tokens.h"
#include "ast-scheme-exporter.h"

void ast_export_root(FILE* dest, struct ast_node_t* node) {
	print_naked_list(dest, node, 0, 1);
	fprintf(dest, "\n");
}

void print_single_node(FILE* dest, struct ast_node_t* node, int padding,
		int wrap) {
	TOKEN_TYPE_T type = node->type;
	if (is_atomic(type)) {
		print_atomic(dest, node, padding, wrap);
	} else {
		print_compozite(dest, node, padding, wrap);
	}
}

void print_atomic(FILE* dest, struct ast_node_t* node, int padding, int wrap) {
	if (wrap) {
		//print_padding(dest, padding);
	}

	switch (node->type) {
	case ATT_NUMBER:
		fprintf(dest, "%ld", node->value.number);
		return;
	case ATT_IDENTIFIER:
		fprintf(dest, "%s", node->value.string);
		return;
	case ATT_STRING:
		fprintf(dest, "\"%s\"", node->value.string);
		return;
	default:
		fprintf(stderr, "unknown type of type to export: %d", node->type);
		return;
	}
}

void print_compozite(FILE* dest, struct ast_node_t* node, int padding, int wrap) {

	ast_node_t* child = node->value.child;

	int new_pad = padding + (wrap ? 1 : 1);

	switch (node->type) {
	//absolutelly syntactically specifically printed
	case CNT_VARS_DECLS:
		print_vars_decls(dest, node, padding, wrap);
		break;
	case JST_PROCCALL:
		print_proccal(dest, node, padding, wrap);
		break;
	case JST_ATOMIC_VARIABLE_DECL:
		print_atomic_var_decl(dest, node, padding, wrap);
		break;
	case JST_ARRAY_VARIABLE_DECL:
		print_array_var_decl(dest, node, padding, wrap);
		break;
	case STK_SIZEOF:
		print_sizeof(dest, node, padding, wrap);
		break;
		// printing of containers
	case CNT_PARAMETERS:
		print_scheme_list(dest, child, new_pad, 0);
		break;
	case CNT_STATEMENTS:
		print_renamed(dest, "begin", child, new_pad, 1);
		break;
	case CNT_EXPRESSIONS:
		print_renamed(dest, "list", child, new_pad, 0);
		break;

		// renaming
	case STK_ASSIGNMENT:
		print_renamed(dest, "set!", child, new_pad, 0);
		break;
	case JST_PROCEDURE:
		print_renamed(dest, "lambda", child->next, new_pad, 1);
		break;
	case OPT_INDEX:
		print_renamed(dest, "at", child, new_pad, 0);
		break;
	case OPT_DEREFERENCE:
		print_renamed(dest, "address-of", child, new_pad, 0);
		break;
	case OPT_REFERENCE:
		print_renamed(dest, "at-address", child, new_pad, 0);
		break;
	case OPT_TERNARY:
		print_renamed(dest, "if", child, new_pad, 0);
		break;
	case OPT_NOT:
		print_renamed(dest, "not", child, new_pad, 0);
		break;
	case OPT_AND:
		print_renamed(dest, "and", child, new_pad, 0);
		break;
	case OPT_OR:
		print_renamed(dest, "or", child, new_pad, 0);
		break;
	case OPT_BITWISE_NOT:
		print_renamed(dest, "bit-not", child, new_pad, 0);
		break;
	case OPT_BITWISE_AND:
		print_renamed(dest, "bit-and", child, new_pad, 0);
		break;
	case OPT_BITWISE_OR:
		print_renamed(dest, "bit-or", child, new_pad, 0);
		break;
	case OPT_BITWISE_XOR:
		print_renamed(dest, "bit-xor", child, new_pad, 0);
		break;
	case OPT_MODULO:
		print_renamed(dest, "mod", child, new_pad, 0);
		break;

		//normally printed, but with wrap
	case STK_IF:
	case STK_WHILE:
	case STK_FOR:
	case STK_DO: {
		const char* str = to_string(node->type);
		print_labeled_scheme_list(dest, str, child, new_pad, 1);
	}
		break;
	default: {
		const char* str = to_string(node->type);
		print_labeled_scheme_list(dest, str, child, new_pad, 0);
	}
	}
}

void print_vars_decls(FILE* dest, struct ast_node_t* node, int padding,
		int wrap) {
	ast_node_t* child = node->value.child;
	int new_pad = padding + 1;
	print_renamed(dest, "define-with-tuples", child, new_pad, 1);

	//or let* if not top-level
	//FIXME TODO
}
void print_proccal(FILE* dest, struct ast_node_t* node, int padding, int wrap) {
	ast_node_t* child = node->value.child;
	int new_pad = padding + 1;

	fprintf(dest, "(");

	print_single_node(dest, child, new_pad, 0);
	print_separator(dest, new_pad, 0);

	print_naked_list(dest, child->next->value.child, new_pad, 0);
	fprintf(dest, ")");
	print_separator(dest, new_pad, wrap);

	//FIXME
}

void print_sizeof(FILE* dest, struct ast_node_t* node, int padding, int wrap) {
	ast_node_t* child = node->value.child;
	int new_pad = padding + 1;

	if (child->type == STK_TYPE) {
		print_labeled_scheme_list(dest, "sizeof-long", NULL, new_pad, 0);
	} else {
		print_labeled_scheme_list(dest, "sizeof", child, new_pad, 0);
	}
}
void print_atomic_var_decl(FILE* dest, struct ast_node_t* node, int padding,
		int wrap) {
	ast_node_t* child = node->value.child;
	int new_pad = padding + 1;

	print_scheme_list(dest, child, new_pad, 1);
}

void print_array_var_decl(FILE* dest, struct ast_node_t* node, int padding,
		int wrap) {
	ast_node_t* child = node->value.child;
	int new_pad = padding + 1;

	fprintf(dest, "(");
	print_single_node(dest, child, new_pad, 0);
	print_separator(dest, new_pad, 0);

	print_renamed(dest, "make-array", child->next, new_pad, 0);

	fprintf(dest, ")");
	print_separator(dest, new_pad, wrap);
}

/*
 int print_head(FILE* dest, struct ast_node_t* node, int padding) {

 fprintf("XXX");
 return 1;
 }
 */

void print_renamed(FILE* dest, const char* head, struct ast_node_t* rest,
		int padding, int wrap) {
	print_labeled_scheme_list(dest, head, rest, padding, wrap);
}

void print_naked_list(FILE* dest, struct ast_node_t* node, int padding,
		int wrap) {
	while (node) {
		print_single_node(dest, node, padding, wrap);
		node = node->next;
		if (node) {
			print_separator(dest, padding, wrap);
		}
	}
}

void print_scheme_list(FILE* dest, struct ast_node_t* node, int padding,
		int wrap) {
	print_labeled_scheme_list(dest, NULL, node, padding, wrap);
}

void print_labeled_scheme_list(FILE* dest, const char* head,
		struct ast_node_t* node, int padding, int wrap) {

	fprintf(dest, "(");
	if (head) {
		fprintf(dest, "%s", head);
		if (node) {
			print_separator(dest, padding, wrap);
		}
	}

	print_naked_list(dest, node, padding, wrap);

	fprintf(dest, ")");

}

void print_separator(FILE* dest, int padding, int wrap) {
	if (wrap) {
		fprintf(dest, "\n");
		print_padding(dest, padding);
	} else {
		fprintf(dest, " ");
	}
}

#endif
