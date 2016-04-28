#ifndef BISON_HEADERS_H
#define BISON_HEADERS_H
#include <stdio.h>
#include "lexer-headers.h"

void yyerror(char *s);


#ifdef SYNTAXER_VERBOSE
#define SYNTAXER_LOG(MSG...) \
	{ \
		printf("[SNT] "); \
		printf(MSG); \
		printf("\n"); \
	}
#else
	#define SYNTAXER_LOG(MSG...)
#endif

#endif
