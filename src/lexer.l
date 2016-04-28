%{
	/**
		Martin Jašek
		KMI UP
		LS 2015
		*/
	#include <stdlib.h>
	#include "../src/lexer-headers.h"
	#include "../gen/syntaxer.h"
%}

%option outfile="gen/lexer.c" header-file="gen/lexer.h"

%%
[0-9]+	{
	LEXER_LOG("Number : %s\n", yytext);
	return TK_NUMBER;
}


[_a-zA-Z]+	{
	LEXER_LOG("Identifier : %s\n", yytext);	
	return TK_VARIABLE;
}

"+"|"-"|"*"|"/"	{
	LEXER_LOG("Operator : %s\n", yytext);
	return TK_OPERATOR;
}

. {
	fprintf(stderr, "LEX: Unknown : %s\n", yytext);
}

<<EOF>> {
	printf("LEX: EOF.\n");
	yyterminate();
}
%%


int yywrap() {
	return 0;	//TODO what here?
}
