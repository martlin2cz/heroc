%{
	
	
	#include "../src/lexer-headers.h"
	
	#include <stdlib.h>
	#include <string.h>
	
%}

%option outfile="gen/lexer.c" header-file="gen/lexer.h"

%%
[0-9]+	{
	LEXER_LOG("Number : %s", yytext);
	yylval = malloc(sizeof(YYSTYPE));
	yylval->str = malloc(10000);
	strcpy(yytext, yylval->str);
	return TK_NUMBER;
}


[_a-zA-Z]+	{
	LEXER_LOG("Identifier : %s", yytext);
	yylval = malloc(sizeof(YYSTYPE));
	yylval->str = malloc(10000);
	strcpy(yytext, yylval->str);
	return TK_VARIABLE;
}

"+"|"-"|"*"|"/"	{
	LEXER_LOG("Operator : %s", yytext);
	yylval = malloc(sizeof(YYSTYPE));
	yylval->str = malloc(10000);
	strcpy(yytext, yylval->str);
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
