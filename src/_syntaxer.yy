%{
#include <stdio.h>
#include "../src/syntaxer-headers.h"
#include "../gen/syntaxer.h"


%}

%output "gen/syntaxer.c"
%defines "gen/syntaxer.h"





%token	TK_NUMBER	'N'
%token TK_VARIABLE	'V'
%token TK_OPERATOR	'O'

%start expr

%%
expr:
	value TK_OPERATOR value {
		SYNTAXER_LOG("operation %s", $2->str);	
	} 
;

value:
	TK_NUMBER {
		SYNTAXER_LOG("number %s", $1->str);
	}
|	TK_VARIABLE {
		SYNTAXER_LOG("variable %s", $1->str);
	}
;	

%%

void yyerror(char *s) {
	fprintf(stderr, "SNT: syntax error: %s\n", s);
}

