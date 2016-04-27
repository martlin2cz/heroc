%{
	/**
		Martin Jašek
		KMI UP
		LS 2015
		*/
	#include <stdlib.h>
	#include "flex-headers.h"
	#include "bison.h"
%}

%option outfile="flex.c" header-file="flex.h"

%%
\n	{
	new_line();
}

[\t ]	{
	readen_char();
}

0((x[0-9a-f]+)|(X[0-9A-F]+))	{
	yylval.number = parse_hex(yytext);
	TOKEN(TK_NUMBER);
}

0[bB][0-1]+	{
	yylval.number = parse_bin(yytext);
	TOKEN(TK_NUMBER);
}

0*[0-9]+	{
	yylval.number = parse_dec(yytext);
	TOKEN(TK_NUMBER);
}

false	{
	yylval.number = 0;
	TOKEN(TK_NUMBER);
}

true	{
	yylval.number = 1;
	TOKEN(TK_NUMBER);
}

'.'	{	//znak, ksakru!
	yylval.number = parse_char(yytext);;
	TOKEN(TK_NUMBER);
}

\"([^\"]|\"\")*\" {
	yylval.string = string_const(yytext, yyleng);
	TOKEN(TK_STRING);	
}

","	TOKEN(TK_COMA);
";"	TOKEN(TK_SEMICOLON);
"(" TOKEN(TK_ROUND_BRACKET_OPEN);
")" TOKEN(TK_ROUND_BRACKET_CLOSE);
"{" TOKEN(TK_CURLY_BRACKET_OPEN);
"}" TOKEN(TK_CURLY_BRACKET_CLOSE);


"+" OPERATOR(TK_OPERATOR_PLUS);  
"-" OPERATOR(TK_OPERATOR_MINUS);
"*" OPERATOR(TK_OPERATOR_TIMES); 
"/" OPERATOR(TK_OPERATOR_DIVIDE); 
"%" OPERATOR(TK_OPERATOR_MODULO);
"++" OPERATOR(TK_OPERATOR_INCREMENT); 
"--" OPERATOR(TK_OPERATOR_DECREMENT); 

"==" OPERATOR(TK_OPERATOR_EQUAL);
"!=" OPERATOR(TK_OPERATOR_NOT_EQUAL); 
"<" OPERATOR(TK_OPERATOR_LESS_THAN); 
">" OPERATOR(TK_OPERATOR_GREATER_THAN); 
"<=" OPERATOR(TK_OPERATOR_LESS_EQUAL);
">=" OPERATOR(TK_OPERATOR_GREATER_EQUAL); 

"!" OPERATOR(TK_OPERATOR_NOT);
"&&" OPERATOR(TK_OPERATOR_AND);
"||" OPERATOR(TK_OPERATOR_OR);

"~" OPERATOR(TK_OPERATOR_BIT_NOT); 
"&" OPERATOR(TK_OPERATOR_BIT_AND);
"|" OPERATOR(TK_OPERATOR_BIT_OR);
"^" OPERATOR(TK_OPERATOR_BIT_XOR);
"<<" OPERATOR(TK_OPERATOR_SHIFT_LEFT); 
">>" OPERATOR(TK_OPERATOR_SHIFT_RIGHT); 

"=" ASSG_WITH_OP(TK_OPERATOR_ASSIGNMENT);
"+=" ASSG_WITH_OP(TK_OPERATOR_PLUS);
"-=" ASSG_WITH_OP(TK_OPERATOR_MINUS);
"*=" ASSG_WITH_OP(TK_OPERATOR_TIMES);
"/=" ASSG_WITH_OP(TK_OPERATOR_DIVIDE);
"%=" ASSG_WITH_OP(TK_OPERATOR_MODULO);
"<<=" ASSG_WITH_OP(TK_OPERATOR_SHIFT_LEFT);
">>=" ASSG_WITH_OP(TK_OPERATOR_SHIFT_RIGHT);
"&=" ASSG_WITH_OP(TK_OPERATOR_BIT_AND);
"^=" ASSG_WITH_OP(TK_OPERATOR_BIT_XOR);
"|=" ASSG_WITH_OP(TK_OPERATOR_BIT_OR);

"if" TOKEN(TK_KEYW_IF);
"else" TOKEN(TK_KEYW_ELSE);
"for" TOKEN(TK_KEYW_FOR);
"do"	TOKEN(TK_KEYW_DO);
"while"	TOKEN(TK_KEYW_WHILE);
"continue"  TOKEN(TK_KEYW_CONTINUE);
"break"  TOKEN(TK_KEYW_BREAK);
"print"  TOKEN(TK_KEYW_PRINT);
"scan"  TOKEN(TK_KEYW_SCAN);

"/*"	{
	return multiline_comment(yytext);
}

"//"	{
	return oneline_comment(yytext);
}

[_a-zA-Z][_a-zA-Z0-9]* {
	yylval.string = variable_name(yytext, yyleng); 
	TOKEN(TK_VARIABLE);
}

. {
	invalid_token(yytext, line, col);
	TOKEN(TK_UNKNOWN);
}
%%
//TODOOOO zeby tohle
int isatty(int f) { 
	return yyin==stdin; 
}


void flush() { 
	YY_FLUSH_BUFFER; 
}

void flex_init() { 
	line = 1; col = 1; 
}

int yywrap() { 
	return 1; 
}

int fileno(FILE* file) {
	return 0;
}

void new_line() {
	line++;
	col = 1;
}

void readen_char() {
	col++;
}

void readen_token() {
	col += yyleng;
}

int parse_bin(char* text) {
	return strtol(text + 2, NULL, 2);
}

int parse_dec(char* text) {
	return atoi(text);
}	

int parse_hex(char* text) {
	return strtol(text + 2, NULL, 16);
}

char parse_char(char* text) {
	return text[1];
}

char* string_const(char* text, size_t length) {
	char* value = (char*) malloc((length - 2) * sizeof(char));
	strncpy(value, yytext + 1, length - 2);	
	return value;
}

token_type multiline_comment(char* text) {
	char z0 = 0;
	unsigned int startline = line;

	while (1) {
		char z1 = NEXT_CHAR();
		readen_char();
	
		if (z1 == EOF) { 
			printf("File ended in comment begining at %d th line\n", startline);
			return TK_MISSING;
		}

		if (z0 == '*' && z1 == '/') {
			break;
		}
	
		z0 = z1;
	
		if (z0 == '\n') {
			new_line();
		}
	}

	return TK_COMMENT;
}

token_type oneline_comment(char* text) {
	while (1) { 
		char z = NEXT_CHAR();
		
		if (z == EOF) {
			return TK_MISSING;
		}

		if (z == '\n') { 
			new_line();
			break; 
		} 
	}

	return TK_COMMENT;
}

char* variable_name(char* text, size_t length) {
	char* varname = (char*) malloc(length * sizeof(char));
	strcpy(varname, text);
	return varname;
}

void invalid_token(char* text, int line, int col) {
	fprintf(stderr, "Lexical error: invalid token starting with \"%s\" at line %d, char %d.\n", text, line, col);
}
