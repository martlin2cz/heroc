#ifndef BISON_HEADERS_H
#define BISON_HEADERS_H
#include <stdio.h>
#include "flex-headers.h"

//void yyerror(const char *s);
void yyerror(struct syntax_node_t **n, char *s);

int verbose_lex = 0;

#define VERB(title, $$, args...)	{	\
	if (verbose_lex) {\
		printf("# %s: ", title);	\
		printf(args); \
		printf(" -> %p\n", $$);	\
	}\
/*	print_root($$); printf("------------------------\n"); */\
}

#define BINARY_OP($$, priority, op, left, right) { \
	  $$ = create_binary(op, left, right); \
	  VERB("binary operator", $$, "(#%d) %p %x %p", priority, left, op, right); \
}

#define UNARY_OP($$, priority, op, content) { \
	  $$ = create_unary(op, content); \
	  VERB("unary operator", $$, "(#%d) %x %p", priority, op, content); \
}


#define TK_OPERATOR_PRE_INC 0x1018
#define TK_OPERATOR_PRE_DEC 0x1019

#endif
