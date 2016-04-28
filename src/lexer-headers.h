#ifndef _LEXER_HEADERS_H
#define _LEXER_HEADERS_H

/**
 Martin Jašek
 KMI UP
 LS 2015
 */
#include "../src/ast.h"

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
