#ifndef _AST_BASIC_EXPORTER_H_
#define _AST_BASIC_EXPORTER_H_

#include <stdio.h>
#include "ast-exporter.h"

int print_single_node(FILE* dest, struct ast_node_t* root, int padding,
		int wrap);

void print_preamble(FILE* dest);
void print_after(FILE* dest);

void print_atomic(FILE* dest, struct ast_node_t* node, int padding, int wrap);
int print_compozite(FILE* dest, struct ast_node_t* node, int padding, int wrap);
//int print_head(FILE* dest, struct ast_node_t* node, int padding);

void print_procedure(FILE* dest, struct ast_node_t* node, int padding,
		int wrap);

int print_variable_decl(FILE* dest, struct ast_node_t* node, int padding,
		int wrap);
void print_array(FILE* dest, struct ast_node_t* node, int padding,
		int wrap);

void print_proccal(FILE* dest, struct ast_node_t* node, int padding, int wrap);
void print_sizeof(FILE* dest, struct ast_node_t* node, int padding, int wrap);

void print_renamed(FILE* dest, const char* head, struct ast_node_t* rest,
		int padding, int wrap);

void print_scheme_list(FILE* dest, struct ast_node_t* node, int padding,
		int wrap);
void print_naked_list(FILE* dest, struct ast_node_t* node, int padding,
		int wrap);
void print_labeled_scheme_list(FILE* dest, const char* head,
		struct ast_node_t* node, int padding, int wrap);
void print_separator(FILE* dest, int padding, int wrap);
//void print_construct(FILE* dest, struct ast_node_t* node, int padding);
//void print_operator(FILE* dest, struct ast_node_t* node, int padding);

#endif
