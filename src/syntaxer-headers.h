#ifndef _SYNTAXER_HEADERS_H_
#define _SYNTAXER_HEADERS_H_



#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ast.h"
#include "tokens.h"


extern int yylex (void);
int yyerror (char *s);


#ifdef SYNTAXER_VERBOSE
#define SYNTAXER_LOG(...) \
	{ \
		printf("[SNT] "); \
		printf(__VA_ARGS__); \
		printf("\n"); \
	}
#else
	#define SYNTAXER_LOG(...)
#endif




#endif
