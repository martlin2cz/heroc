#ifndef _LEXER_HEADERS_H_
#define _LEXER_HEADERS_H_

#include "../src/token.h"
#include "../src/ast.h"
#include "../gen/syntaxer.h"

int yylex(void);
int fileno(FILE* file);
int yywrap(void);

#ifdef LEXER_VERBOSE
#define LEXER_LOG(MSG...) \
	{ \
		printf("[LEX] "); \
		printf(MSG); \
		printf("\n"); \
	}
#else
	#define LEXER_LOG(MSG...)
#endif

#endif
