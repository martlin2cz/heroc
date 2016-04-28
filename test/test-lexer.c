#include <stdio.h>
#include "../gen/lexer.h"

int main(int argc, char **argv)
{
	printf("Running lexer (stdin): \n");

	yyin = stdin;
    yylex();

    printf("Done.\n");

    return 0;
}
