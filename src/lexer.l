%{
#include "../src/lexer-headers.h"
%}

%option outfile="gen/lexer.c" header-file="gen/lexer.h"
%option noyywrap

DIGIT	[0-9]
HEXDIGIT	[0-9a-fA-F]
ALPHA	[a-zA-Z]
ALNUM	DIGIT|ALPHA
IDCHAR	[_a-zA-Z]
OPER	[\+\*\/\%]
MULTIOPER	[\-\*]
BRACKET	[\(\)\{\}\[\]]

%%

{DIGIT}+ {
	LEXER_LOG("decimal number: %s" , yytext);
    yylval.number = parse_dec(yytext);
    return NT_NUMBER;
}

0x{HEXDIGIT}+ {
	LEXER_LOG("hex number: %s" , yytext);
    yylval.number = parse_hex(yytext);
    return NT_NUMBER;
}

{ALPHA}{IDCHAR}* {
	LEXER_LOG("identifier: %s" , yytext);
	yylval.string = parse_identifier(yytext);
    return NT_IDENTIFIER;
}

{OPER} {
	LEXER_LOG("operator: %s" , yytext);
	int oper = str_to_oper(yytext);
    return oper;
}

{MULTIOPER} {
	LEXER_LOG("operator: %s" , yytext);
	int oper = str_to_multioper(yytext);
	yylval.oper = oper;
    return oper;
}

{BRACKET} {
	LEXER_LOG("bracket: %s" , yytext);
	int bra = str_to_bracket(yytext);
    return bra;
}

";" {
	LEXER_LOG("end of statement");
    return NT_SEMICOLON;
}

[ \t\n]+ {
	LEXER_LOG("just whitespace: %s", yytext);
}

. {
	fprintf(stderr, "Lexical error. Unknown token starting by %s at line %d. Skipping. \n", yytext, yylineno);
}

%%

#include "../src/lexer-impl.c"
