#ifndef _AST_BASIC_EXPORTER_H_
#define _AST_BASIC_EXPORTER_H_

#include <stdio.h>
#include "ast-exporter.h"
#include "ast-displayer.h"


void ast_export_root(FILE* dest, struct ast_node_t* root);
void print_preamble(FILE* dest);
void print_after(FILE* dest);

#endif
