%{
#include "../src/lexer-headers.h"
%}

%option outfile="gen/lexer.c" header-file="gen/lexer.h"
%option noyywrap

DIGIT		[0-9]
HEXDIGIT	[0-9a-fA-F]
OCTDIGIT	[0-7]
ALPHA		[a-zA-Z]
ALNUM		DIGIT|ALPHA

OPER		[\+\*\/\%]|(\|\|)|(\&\&)|(\!)|(\|)|(\&)|(\^)|(\~)|(\<)|(\>)|(\<\=)|(\>\=)|(\!=)|(\<\<)|(\>\>)
MULTIOPER	(\-)|(\+)|(\*)|(\+\+)|(\-\-)
ASGOPER		(\=)|(\+\=)|(\-\=)|(\*\=)|(\/\=)(\%\=)|(\|\=)|(\&\=)|(\!\=)|(\^\=)|(\~\=)|(\<\<=)|(\>\>=)
BRACKET		[\(\)\{\}\[\]]
SPECSYM		[\,\;\:\?]

KEYWORD 	long|sizeof|if|else|for|while|do|return|break|continue|goto|lambda

%x MULTILINE_COMMENT
%x ONELINE_COMMENT
%x STRING_LITERAL


%%

{KEYWORD} {
	LEXER_LOG("keyword: %s" , yytext);
	int keyw = str_to_keyword(yytext);
    return keyw;
}

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

(_|{ALPHA})(_|{ALPHA}|{DIGIT})* {
	LEXER_LOG("identifier: %s" , yytext);
	yylval.string = parse_identifier(yytext);
    return NT_IDENTIFIER;
}


{MULTIOPER} {
	LEXER_LOG("multioperator: %s" , yytext);
	int oper = str_to_multioper(yytext);
    return oper;
}

{OPER} {
	LEXER_LOG("operator: %s" , yytext);
	int oper = str_to_oper(yytext);
    return oper;
}

{ASGOPER} {
	LEXER_LOG("assignment: %s" , yytext);
	int asgop = str_to_asgoper(yytext);
    return TT_ASSIGNMENT;
}


{BRACKET} {
	LEXER_LOG("bracket: %s" , yytext);
	int bra = str_to_bracket(yytext);
    return bra;
}


{SPECSYM} {
	LEXER_LOG("special symbol: %s", yytext);
    int ssym = str_to_specsym(yytext);
    return ssym;
}

\'((\\)?(.))\' {
	LEXER_LOG("character: %s", yytext);
	long c = parse_character(yytext);
	yylval.number = c;
	return NT_NUMBER;
}

\" {
	LEXER_LOG("start of string: %s", yytext);
	BEGIN(STRING_LITERAL);
}

<STRING_LITERAL>(\\{2}|[^\"])* {
	LEXER_LOG("string constant: %s", yytext);
	yylval.string = parse_string(yytext);
	return NT_STRING;
}

<STRING_LITERAL>\" {
	LEXER_LOG("end of string: %s", yytext);
	BEGIN(INITIAL);
}

"//" {
	LEXER_LOG("start of one-line comment: %s", yytext);
	BEGIN(ONELINE_COMMENT);
}


<ONELINE_COMMENT>[^\n]*\n {
	LEXER_LOG("one-line comment: %s", yytext);
	BEGIN(INITIAL);
}

"/*" {
	LEXER_LOG("start of multi-line comment: %s", yytext);
	BEGIN(MULTILINE_COMMENT);
}

<MULTILINE_COMMENT>[^*]+|"*"+[^/] {
	LEXER_LOG("multi-line comment's body: %s", yytext);
}

<MULTILINE_COMMENT>"*"+"/" {
    LEXER_LOG("end of multi-line comment: %s", yytext);
    BEGIN(INITIAL);
}


[ \t\n]+ {
	LEXER_LOG("just whitespace: \"%s\"", yytext);
}

<<EOF>> {
	LEXER_LOG("end of input");
	yyterminate();
}

. {
	fprintf(stderr, "Lexical error. Unknown token starting by %s at line %d. Skipping. \n", yytext, yylineno);
}

%%

#include "../src/lexer-impl.c"
