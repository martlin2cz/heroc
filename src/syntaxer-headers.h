#ifndef _SYNTAXER_HEADERS_H_
#define _SYNTAXER_HEADERS_H_

#include <stdio.h>
#include "token.h"

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
