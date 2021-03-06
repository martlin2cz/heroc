#ifndef _SYNTAXER_IMPL_C
#define _SYNTAXER_IMPL_C

#include "syntaxer-headers.h"
#include "ast-displayer.h"

int yyerror(struct ast_node_t** tree, char *s) {
	fprintf(stderr, "Syntax error: %s, somewhere near to: \n", s);
	ast_display_root(stderr, *tree);
	*tree = NULL;
	return 0;
}

#endif
