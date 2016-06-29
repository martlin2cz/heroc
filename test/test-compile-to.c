#include <stdio.h>

#include "../gen/lexer.h"
#include "../gen/syntaxer.h"
#include "../src/semanter.h"

#include "../src/stackode.h"
#include "../src/ast-displayer.h"
#include "../src/ast-stackode-exporter.h"

int main(int argc, char **argv) {
	fprintf(stderr, "Running testing compiler: \n");

	yyin = stdin;

	struct ast_node_t* root = NULL;
	yyparse(&root);

	if (!root) {
		fprintf(stderr, "Parsing failed.\n");
		return 1;
	}

	fprintf(stderr, "Parsed, analysing.\n");
	int errors = analyze_tree(root);
	//ast_display_root(stdout, root);

	if (errors) {
		fprintf(stderr, "Analysing failed, %d errors found. \n", errors);
		return 2;
	}

	ast_export_root(stdout, root);

	fprintf(stderr, "Done. Yikes!\n");

	return 0;
}

