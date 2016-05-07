#ifndef _LEXER_HEADERS_H_
#define _LEXER_HEADERS_H_

#include "../src/token.h"
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



YYSTYPE
make_double (double value);


#endif
