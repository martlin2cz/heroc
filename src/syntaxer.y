%{

#include "../src/syntaxer-headers.h"

%}

%output "gen/syntaxer.c"
%defines "gen/syntaxer.h"

%code requires
{
#include "../src/ast.h"
}

%union {
	long number;
	char* string;
	int keyword;
	int oper;
	ast_node_t* child;
}


/* deklarace tokenu a jejich priorit */
%token<number> NT_NUMBER
%token<string> NT_IDENTIFIER
%token NT_SEMICOLON
%token<oper> '+' '-' '*' '/' '^' UNM
%type<child> expr
 
%left '-' '+'  /* asociativni zleva */
%left '*' '/' 
%left UNM      /* unarni minus */
%right '^'     /* asociativni zprava */
     
%start program
%%

/* odvozovaci pravidla */

program: 
	stmt
;
 
stmt:
	  NT_SEMICOLON
	| expr NT_SEMICOLON	{ 
		SYNTAXER_LOG("expr %p ;", $1);
	}
;
 
 expr:   
  	  expr '+' expr      { $$ = create_binary($2, $1, $3); 	SYNTAXER_LOG("plus  of %p %p -> %p", $1, $3, $$);	}
	| expr '-' expr      { $$ = create_binary($2, $1, $3);	SYNTAXER_LOG("minus of %p %p -> %p", $1, $3, $$);	}
	| expr '*' expr      { $$ = create_binary($2, $1, $3);	SYNTAXER_LOG("times of %p %p -> %p", $1, $3, $$);	}
	| expr '/' expr      { $$ = create_binary($2, $1, $3);	SYNTAXER_LOG("div   of %p %p -> %p", $1, $3, $$);	}
	| '-' expr %prec UNM { $$ = create_unary($1, $2);		SYNTAXER_LOG("unary minus of %p -> %p", $2, $$);	}
	| expr '^' expr      { $$ = create_binary($2, $1, $3);	SYNTAXER_LOG("pow   of %p %p -> %p", $1, $3, $$);	}
	| '(' expr ')'       { $$ = $2;							SYNTAXER_LOG("( %p ) -> %p", $2, $$);	}
	| NT_NUMBER          { $$ = create_number($1);			SYNTAXER_LOG("number %p -> %p", $1, $$);	}
	| NT_IDENTIFIER      { $$ = create_identifier($1);		SYNTAXER_LOG("identifier %p -> %p", $1, $$);	}
;

%%

#include "../src/syntaxer-impl.c"