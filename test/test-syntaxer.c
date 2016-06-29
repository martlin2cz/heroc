#include <stdio.h>

#include "../gen/lexer.h"
#include "../gen/syntaxer.h"
#include "../src/ast-displayer.h"


int main(int argc, char **argv)
{
	fprintf(stderr, "Running syntaxer (stdin): \n");


	yyin = stdin;

	struct ast_node_t* root = NULL;
    yyparse(&root);

    if (root) {
    	fprintf(stderr, "Done with root %p:\n", root);
    	ast_display_root(stdout, root);
    } else {
    	fprintf(stderr, "Parsing failed.\n");
    }

    fprintf(stderr, "Done.\n");

    return 0;
}
