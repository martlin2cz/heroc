#ifndef _SYNTAXER_HEADERS_H_
#define _SYNTAXER_HEADERS_H_



#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ast.h"
#include "tokens.h"


extern int yylex (void);
int yyerror (struct ast_node_t** tree, char *s);


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


#define BINARY_OP($1, $3, $$, OP) \
	{ \
		*root = $$ = create_binary(OP, $1, $3); \
		SYNTAXER_LOG("%s of %p %p -> %p", to_string(OP), $1, $3, $$); \
	}

#define UNARY_OP($2, $$, OP) \
	{ \
		*root = $$ = create_unary(OP, $2); \
		SYNTAXER_LOG("%s of %p -> %p", to_string(OP), $2, $$); \
	}


#endif
