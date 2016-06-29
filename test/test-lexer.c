#include <stdio.h>
#include "../gen/lexer.h"

int main(int argc, char **argv)
{
	fprintf(stderr, "Running lexer (stdin): \n");

	yyin = stdin;

	int val;
	do {
    	val = yylex();
    } while (val);

    fprintf(stderr, "Done.\n");

    return 0;
}
