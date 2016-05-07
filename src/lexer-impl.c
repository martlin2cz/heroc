#ifndef _LEXER_IMPL_C_
#define _LEXER_IMPL_C_

#include <stdlib.h>
#include <string.h>
#include "lexer-headers.h"

long parse_dec(char* yylval) {
	return atoi(yylval);	//TODO
}
long parse_hex(char* yylval) {
	return atoi(yylval);	//TODO
}
long parse_oct(char* yylval) {
	return atoi(yylval);	//TODO
}

char* parse_identifier(char* yylval) {
	return duplicate_str(yylval);
}

char* duplicate_str(char* yylval) {
	size_t len = strlen(yylval) + 1;
	char* copy = (char*) malloc(len * sizeof(char));
	if (!copy) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	strcpy(copy, yylval);
	return copy;
}

#endif
