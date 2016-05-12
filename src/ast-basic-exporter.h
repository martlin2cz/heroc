#ifndef _AST_BASIC_EXPORTER_H_
#define _AST_BASIC_EXPORTER_H_

#include <stdio.h>
#include "ast-exporter.h"

void print_node(FILE* dest, struct ast_node_t* root, int padding);

void print_atomic(FILE* dest, struct ast_node_t* node, int padding);
void print_compozite(FILE* dest, struct ast_node_t* node, int padding);
//void print_construct(FILE* dest, struct ast_node_t* node, int padding);
//void print_operator(FILE* dest, struct ast_node_t* node, int padding);

#endif
