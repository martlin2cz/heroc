#ifndef _AST_BASIC_EXPORTER_C_
#define _AST_BASIC_EXPORTER_C_

#include <stdio.h>

#include <unistd.h>
#include "misc.h"
#include "ast.h"
#include "tokens.h"
#include "ast-scheme-exporter.h"

void ast_export_root(FILE* dest, struct ast_node_t* node) {
	print_preamble(dest);
	print_naked_list(dest, node, 0, 1);
	print_after(dest);
}

void print_preamble(FILE* dest) {
	fprintf(dest, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; \n");
	fprintf(dest, ";; scheme code generated by heroc compiler\n");
	fprintf(dest, ";; \n");
	fprintf(dest, "\n");
}

void print_after(FILE* dest) {
	fprintf(dest, "\n");
	fprintf(dest, ";; \n");
	fprintf(dest, ";; end of generated code\n");
	fprintf(dest, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; \n");
}

int print_single_node(FILE* dest, struct ast_node_t* node, int padding,
		int wrap) {
	TOKEN_TYPE_T type = node->type;

	if (is_meta(type)) {
		//ignore

	} else if (is_atomic(type)) {
		print_atomic(dest, node, padding, wrap);

	} else {
		return print_compozite(dest, node, padding, wrap);
	}

	return 0;
}

void print_atomic(FILE* dest, struct ast_node_t* node, int padding, int wrap) {
	if (wrap) {
		print_padding(dest, padding);
	}

	switch (node->type) {
	case ATT_NUMBER:
		fprintf(dest, "%ld", node->value.number);
		return;
	case ATT_STRING:
		fprintf(dest, "%s", node->value.string);
		return;
	default:
		fprintf(stderr, "unknown type of type to export: %d", node->type);
		return;
	}
}

int print_compozite(FILE* dest, struct ast_node_t* node, int padding, int wrap) {

	ast_node_t* child = node->value.child;

	int new_pad = padding + (wrap ? 1 : 1);	//TODO lol, wtf?

	switch (node->type) {
	//absolutelly syntactically specifically printed
	case JST_PROGRAM:
		print_whole_program(dest, child, padding, wrap);
		break;
	case JST_VARIABLE_DECL: {
		int done = print_variable_decl(dest, node, padding, wrap);
		return done;
		break;
	}
	case JST_VARIABLE: {
		print_single_node(dest, child, padding, wrap);
		break;
	}
	case JST_PROCEDURE: {
		print_procedure(dest, node, padding, wrap);
		return 1;
		break;
	}
	case JST_PROCCALL:
		print_proccal(dest, node, padding, wrap);
		break;
	case JST_ARRAY:
		print_array(dest, node, padding, wrap);
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

	case JST_EXPRESSION:
		print_single_node(dest, child, new_pad, 0);
		break;
		// renaming
	case STK_ASSIGNMENT:
		print_renamed(dest, "set-at", child, new_pad, 0);
		break;
	case OPT_INDEX:
		print_renamed(dest, "at", child, new_pad, 0);
		break;
	case OPT_DEREFERENCE:
		print_renamed(dest, "adress-of", child, new_pad, 0);
		break;
	case OPT_REFERENCE:
		print_renamed(dest, "at-adress", child, new_pad, 0);
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
	case STK_DO:
		print_renamed(dest, "do-while", child, new_pad, 1);
		break;

		//normally printed, but with wrap
	case STK_IF:
	case STK_WHILE:
	case STK_FOR: {
		const char* str = to_string(node->type);
		print_labeled_scheme_list(dest, str, child, new_pad, 1);
	}
		break;
	default: {	//print inline (operators)
		const char* str = to_string(node->type);
		print_labeled_scheme_list(dest, str, child, new_pad, 0);
	}
	}
	return 0;
}

void print_whole_program(FILE* dest, struct ast_node_t* decls, int padding, int wrap) {
	print_naked_list(dest, decls, padding, wrap);
}

void print_procedure(FILE* dest, struct ast_node_t* node, int padding, int wrap) {
	ast_node_t* name = node->value.child;
	ast_node_t* params = name->next;
	ast_node_t* body = params->next;
	int new_pad = padding + 1;

	fprintf(dest, "(");
	fprintf(dest, "lambda");
	print_separator(dest, new_pad, 0);

	fprintf(dest, "(");
	ast_node_t* param = params->value.child;

	while (param) {
		if (!is_meta(param->type)) {
			print_single_node(dest, param->value.child, new_pad, 0);
			print_separator(dest, new_pad, 0);
		}
		param = param->next;
	}
	fprintf(dest, ")");
	print_separator(dest, new_pad, 1);

	print_single_node(dest, body, new_pad, 1);
	fprintf(dest, ")");
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

	//FIXME ?
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

int print_variable_decl(FILE* dest, struct ast_node_t* node, int padding,
		int wrap) {

	ast_node_t* ident = node->value.child;
	ast_node_t* value = node->value.child->next;
	int new_pad = padding + 1;
	int is_global = (padding == 0);

	fprintf(dest, "(");

	if (is_global) {
		fprintf(dest, "define");
	} else {
		fprintf(dest, "let");
	}

	print_separator(dest, new_pad, 0);

	if (!is_global) {
		fprintf(dest, "(");
		fprintf(dest, "(");
	}

	print_single_node(dest, ident, new_pad, 0);
	print_separator(dest, new_pad, 0);

	if (value && !is_meta(value->type)) {
		print_single_node(dest, value, new_pad, 0);
	} else {
		fprintf(dest, "'undefined");
	}

	int completed;
	if (!is_global) {
		fprintf(dest, ")");
		fprintf(dest, ")");

		print_separator(dest, new_pad, 1);
		print_naked_list(dest, node->next, new_pad, wrap);

		completed = 1;
	} else {
		completed = 0;
	}

	fprintf(dest, ")");

	return completed;
}

void print_array(FILE* dest, struct ast_node_t* node, int padding, int wrap) {
	ast_node_t* child = node->value.child;
	int new_pad = padding + 1;

	print_renamed(dest, "make-array", child, new_pad, 0);
}

void print_renamed(FILE* dest, const char* head, struct ast_node_t* rest,
		int padding, int wrap) {
	print_labeled_scheme_list(dest, head, rest, padding, wrap);
}

void print_naked_list(FILE* dest, struct ast_node_t* node, int padding,
		int wrap) {
	while (node) {
		int skip = print_single_node(dest, node, padding, wrap);
		if (skip) {
			break;
		}
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
