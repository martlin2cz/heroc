#include <stdio.h>

#include "../gen/lexer.h"
#include "../gen/syntaxer.h"

int main(int argc, char **argv)
{
	printf("Running syntaxer (stdin): \n");

    yyparse();

    printf("Done.\n");

    return 0;
}
