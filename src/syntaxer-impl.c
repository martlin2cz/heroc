#ifndef _SYNTAXER_IMPL_C
#define _SYNTAXER_IMPL_C

#include "syntaxer-headers.h"
#include "ast-exporter.h"

int yyerror (struct ast_node_t** tree, char *s) {
    fprintf(stderr, "Syntax error: %s near to:\n", s);
    ast_export_root(stderr, *tree);
    *tree = NULL;
    return 0;
}

#endif
