#ifndef _AST_BASIC_EXPORTER_H_
#define _AST_BASIC_EXPORTER_H_

#include <stdio.h>
#include "ast.h"

void ast_display_root(FILE* dest, struct ast_node_t* node);
void ast_display_nodes(FILE* dest, struct ast_node_t* nodes, int padding);
void ast_display_single_node(FILE* dest, struct ast_node_t* node, int padding);

void ast_display_atomic(FILE* dest, struct ast_node_t* node, int padding);
void ast_display_meta(FILE* dest, struct ast_node_t* node, int padding);
void ast_display_compozite(FILE* dest, struct ast_node_t* node, int padding);

#endif
