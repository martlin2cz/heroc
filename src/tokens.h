#ifndef _TOKENS_H_
#define _TOKENS_H_

#define TOKEN_TYPE_T int

#include "../gen/syntaxer.h"

/** QUERYING **/
int is_atomic(TOKEN_TYPE_T type);
int is_operator(TOKEN_TYPE_T type);
int is_construct(TOKEN_TYPE_T type);

/** CONVERTING **/
TOKEN_TYPE_T str_to_oper(char* str);
char* oper_to_str(TOKEN_TYPE_T oper);

TOKEN_TYPE_T str_to_multioper(char* str);
char* multioper_to_str(TOKEN_TYPE_T moper);

TOKEN_TYPE_T str_to_asgoper(char* str);
char* asgoper_to_str(TOKEN_TYPE_T asgoper);

TOKEN_TYPE_T str_to_bracket(char* str);
char* bracket_to_str(TOKEN_TYPE_T bra);

TOKEN_TYPE_T str_to_specsym(char* str);
char* specsym_to_str(TOKEN_TYPE_T specsym);

TOKEN_TYPE_T str_to_keyword(char* str);
char* keyword_to_str(TOKEN_TYPE_T keyw);

/** HELP METHODS **/
TOKEN_TYPE_T one_char_str_to_oper(char* str, char c1);
TOKEN_TYPE_T two_char_str_to_oper(char* str, char c1, char c2);

TOKEN_TYPE_T one_char_str_to_moper(char* str, char c1);
TOKEN_TYPE_T two_char_str_to_moper(char* str, char c1, char c2);

TOKEN_TYPE_T one_char_str_to_asgoper(char* str, char c1);
TOKEN_TYPE_T two_char_str_to_asgoper(char* str, char c1, char c2);
TOKEN_TYPE_T three_char_str_to_asgoper(char* str, char c1, char c2, char c3);

/** ERRORS MACROS **/
#define UNSUPPORTED 0

#define UNSUPPORTED_STR(str) { \
		fprintf(stderr, "(%3d) Unsupported symbol: %s", __LINE__, str); \
		return UNSUPPORTED; \
	}

#define UNSUPPORTED_TYPE(type) { \
		fprintf(stderr, "(%3d) Unsupported symbol: %d", __LINE__, type); \
		return UNSUPPORTED; \
	}
#endif
