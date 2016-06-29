#include <stdio.h>

#include "../gen/lexer.h"
#include "../gen/syntaxer.h"
#include "../src/semanter.h"

#include "../src/stackode.h"
#include "../src/ast-displayer.h"
#include "../src/ast-stackode-exporter.h"

int main(int argc, char **argv) {
	printf("Running syntaxer, semanter and stackode generator (stdin): \n");

	yyin = stdin;

	struct ast_node_t* root = NULL;
	yyparse(&root);

	if (!root) {
		printf("Parsing failed.\n");
		return 1;
	}

	printf("Parsed, analysing.\n");
	int errors = analyze_tree(root);
	//ast_display_root(stdout, root);

	if (errors) {
		printf("Analysing failed, %d errors found. \n", errors);
		return 2;
	}

	printf("Analysed, generating stackode\n");
	sk_program_t *stackode = ast_to_stackode(root);

	printf("Generated (%d lines), rendering:\n", stackode->count);

	export_stackode(stdout, stackode);

	printf("End.\n");
	return 0;
}

