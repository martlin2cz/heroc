%{

#include "../src/syntaxer-headers.h"

%}

%output "gen/syntaxer.c"
%defines "gen/syntaxer.h"


/* deklarace tokenu a jejich priorit */
%token ENDSTMT NUMBER
%left '-' '+'  /* asociativni zleva */
%left '*' '/' 
%left UNM      /* unarni minus */
%right '^'     /* asociativni zprava */
     
%%

/* odvozovaci pravidla */

    input: /* epsilon */
           | input stmt
           ;
     
     stmt:   ENDSTMT
           | expr ENDSTMT       { SYNTAXER_LOG("expr %p ;", $1);
           							print_tree ($1);
                                  printf ("\t%.10g\n", eval_tree ($1));
                                  free_tree ($1); }
           ;
     
     expr:   expr '+' expr      { SYNTAXER_LOG("plus  of %p %p", $1, $3); $$ = make_op (OP_ADD, $1, $3); }
           | expr '-' expr      { SYNTAXER_LOG("minus of %p %p", $1, $3); $$ = make_op (OP_DIF, $1, $3); }
           | expr '*' expr      { SYNTAXER_LOG("times of %p %p", $1, $3); $$ = make_op (OP_MUL, $1, $3); }
           | expr '/' expr      { SYNTAXER_LOG("div   of %p %p", $1, $3); $$ = make_op (OP_DIV, $1, $3); }
           | '-' expr %prec UNM { SYNTAXER_LOG("unary minus of %p", $1); $$ = make_op (OP_INV, $2, NULL); }
           | expr '^' expr      { SYNTAXER_LOG("pow   of %p %p", $1, $3); $$ = make_op (OP_POW, $1, $3); }
           | '(' expr ')'       { SYNTAXER_LOG("( %p )", $2); $$ = $2; }
           | NUMBER             { SYNTAXER_LOG("number %p", $1); $$ = $1; }
           ;

%%

#include "../src/syntaxer-impl.c"