#ifndef _SYNTAXER_HEADERS_H_
#define _SYNTAXER_HEADERS_H_


#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern int yylex (void);
int yyerror (char *s);
double eval_tree (YYSTYPE expr);
void print_tree (YYSTYPE expr);
void free_tree (YYSTYPE expr);

YYSTYPE
make_op (int op, YYSTYPE left, YYSTYPE right);

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
