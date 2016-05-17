#ifndef _AST_STACKODE_EXPORTER_H_
#define _AST_STACKODE_EXPORTER_H_

#include <stdio.h>
#include "ast-exporter.h"
#include "stackode.h"

void ast_export_root(FILE* dest, struct ast_node_t* root);
void export_stackode_to_gas(FILE* dest, sk_program_t* program);

#endif
