#ifndef _TOKENS_H_
#define _TOKENS_H_

#define TOKEN_TYPE_T int

#include "../gen/syntaxer.h"

typedef struct token_str_tuple_t {
	TOKEN_TYPE_T token;
	const char* str;
} token_str_tuple_t;



TOKEN_TYPE_T find_lexical(char* str);
TOKEN_TYPE_T find_semantic(char* str);

const char* to_string(TOKEN_TYPE_T tok);

int is_atomic(TOKEN_TYPE_T tok);
int is_container(TOKEN_TYPE_T tok);
int is_meta(TOKEN_TYPE_T tok);

#endif
