#ifndef _LEXER_IMPL_C_
#define _LEXER_IMPL_C_

#include <stdlib.h>
#include <string.h>
#include "lexer-headers.h"

long parse_dec(char* yylval) {
	return atol(yylval);
}
long parse_hex(char* text) {
	return strtol(text + 2, NULL, 16);
}

long parse_oct(char* text) {
	return strtol(text + 1, NULL, 8);
}

char* parse_identifier(char* yylval) {
	return duplicate_str(yylval);
}

char* parse_string(char* yylval) {
	return duplicate_str(yylval);
}

long parse_character(char* yylval) {
	if (strlen(yylval) == 3) {
		return yylval[1];
	} else if (strlen(yylval) == 4 && yylval[1] == '\\') {
		switch (yylval[3]) {
		case '\'':
			return '\'';
		case 'n':
			return '\n';
		case 't':
			return '\t';
		case '0':
			return '\0';
		case '\\':
			return '\\';
		}
	}

	fprintf(stderr, "Unsupported char: %s\n", yylval);
	return 0;
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
