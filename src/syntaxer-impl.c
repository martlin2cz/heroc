#ifndef _SYNTAXER_IMPL_C
#define _SYNTAXER_IMPL_C

#include "syntaxer-headers.h"

int yyerror (char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
    return 0;
}

#endif
