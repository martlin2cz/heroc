%{
#include <stdio.h>
#include "../src/syntaxer-headers.h"
//TODO rly?
#include "../gen/syntaxer.h"
%}

%output "gen/syntaxer.c"
%defines "gen/syntaxer.h"

%define api.value.type {int}

%token<number>	TK_NUMBER	'N'
%token<string> TK_VARIABLE	'V'
%token<string> TK_OPERATOR	'O'

%start expr

%%
expr:
	value TK_OPERATOR value {
		SYNTAXER_LOG("SNT: operation \n");	
	} 
;

value:
	TK_NUMBER {
		SYNTAXER_LOG("SNT: number \n");
	}
|	TK_VARIABLE {
		SYNTAXER_LOG("SNT: variable \n");
	}
;	

%%

void yyerror(char *s) {
	fprintf(stderr, "SNT: syntax error: %s\n", s);
}

