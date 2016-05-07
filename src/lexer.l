%{
#include "../src/lexer-headers.h"
%}

%option outfile="gen/lexer.c" header-file="gen/lexer.h"
%option noyywrap

DIGIT     [0-9]
FIXNUM    {DIGIT}{1,20}
DIGITS    {DIGIT}+
EXP       [eE][+-]?{DIGITS}
FRAC      ({DIGITS}?"."{DIGITS})|{DIGITS}"."
FLOAT     ({FRAC}{EXP}?)|({DIGITS}{EXP})
OPER	  [\(\)\+\-\*\/\^]

%%

{FIXNUM}|{FLOAT} {
    yylval = make_double (atof (yytext));
    LEXER_LOG("double: %s" , yytext);
    return NUMBER;
}

{OPER} {
	LEXER_LOG("operator: %s" , yytext);
    return yytext [0];
}

";" {
	LEXER_LOG("end of statement");
    return ENDSTMT;
}

[ \t\n]+ {
    /* ignore whitespace */
}

%%

#include "../src/lexer-impl.c"
