#include <stdio.h>

#include "../gen/lexer.h"
#include "../gen/syntaxer.h"
#include "../src/ast-exporter.h"


int main(int argc, char **argv)
{
	printf("Running syntaxer (stdin): \n");

	struct ast_node_t* root = NULL;
    yyparse(&root);

    if (root) {
    	printf("Done with root %p:\n", root);
    	ast_export_root(stdout, root);
    } else {
    	printf("Parsing failed.\n");
    }

    return 0;
}
