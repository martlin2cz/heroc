#ifndef _AST_BASIC_EXPORTER_C_
#define _AST_BASIC_EXPORTER_C_

#include <stdio.h>

#include <unistd.h>

#include "ast-basic-exporter.h"
#include "misc.h"
#include "ast.h"
#include "tokens.h"
#include "ast-displayer.h"


void ast_export_root(FILE* dest, struct ast_node_t* node) {

	print_preamble(dest);
	ast_display_root(dest, node);
	print_after(dest);
}



void print_preamble(FILE* dest) {
	fprintf(dest, "Tree: \n");
}

void print_after(FILE* dest) {
	fprintf(dest, "End of tree.\n");
}
#endif
