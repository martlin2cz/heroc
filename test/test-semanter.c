#include <stdio.h>

#include "../gen/lexer.h"
#include "../gen/syntaxer.h"
#include "../src/semanter.h"
#include "../src/ast-displayer.h"


int main(int argc, char **argv)
{
	printf("Running syntaxer & semanter (stdin): \n");


	yyin = stdin;

	struct ast_node_t* root = NULL;
    yyparse(&root);

    if (!root) {
    	printf("Parsing failed.\n");
        return 1;
    }

	printf("Parsed, analysing.\n");
	int errors = analyze_tree(root);

	if (errors) {
		printf("Analysing failed, %d errors found. Analysed tree would be corrupted or uncomplete:\n", errors);
		ast_display_root(stdout, root);
		return 2;
	}

	printf("Analysed: \n");
	ast_display_root(stdout, root);

    return 0;
}
