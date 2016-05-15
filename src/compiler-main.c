#ifndef _COMPILER_MAIN_C_
#define _COMPILER_MAIN_C_

#include <stdio.h>
#include "compiler-main.h"
#include "../gen/lexer.h"
#include "../gen/syntaxer.h"
#include "ast-exporter.h"

int main(int argc, char* argv[]) {
	//TODO handle --help and --version
	//TODO handle read from file
	FILE* src = stdin;
	FILE* dest = stdout;

	yyin = src;

	struct ast_node_t* root = NULL;
	yyparse(&root);

	if (root) {
		ast_export_root(dest, root);
	}
}

#endif
