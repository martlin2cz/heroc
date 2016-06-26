#ifndef _AST_BASIC_EXPORTER_C_
#define _AST_BASIC_EXPORTER_C_

#include <stdio.h>

#include <unistd.h>
#include "misc.h"
#include "ast.h"
#include "tokens.h"
#include "ast-displayer.h"
#include "ast-basic-exporter.h"

void ast_export_root(FILE* dest, struct ast_node_t* node) {

	ast_display_root(dest, node);
}

#endif
