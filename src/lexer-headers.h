#ifndef _LEXER_HEADERS_H_
#define _LEXER_HEADERS_H_

//#include "../src/token.h"
//#define YYSTYPE ast_node_t*
#include "../src/ast.h"
#include "../src/tokens.h"

#include "../gen/syntaxer.h"


int yylex(void);
int fileno(FILE* file);
//int yywrap(void);


#ifdef LEXER_VERBOSE
#define LEXER_LOG(...) \
	{ \
		printf("[LEX] "); \
		printf(__VA_ARGS__); \
		printf("\n"); \
	}
#else
	#define LEXER_LOG(...)
#endif


long parse_dec(char* yylval);
long parse_hex(char* yylval);
long parse_oct(char* yylval);
char* parse_identifier(char* yylval);

char* duplicate_str(char* yylval);

#endif
