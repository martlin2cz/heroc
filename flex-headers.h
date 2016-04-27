#ifndef FLEX_HEADERS_H
#define FLEX_HEADERS_H

	/**
		Martin Jašek
		KMI UP
		LS 2015
		*/
	#include <stdlib.h>
	#define YY_SKIP_YYWRAP
	
	#define TOKEN(type) {	\
		readen_token();	\
		return type;	\
	}

	#define OPERATOR(operator_token) {	\
		readen_token();	\
		return operator_token;	\
	}

	#define ASSG_WITH_OP(assg_operator) {	\
		readen_token();	\
		yylval.operator = assg_operator; \
		return TK_OPERATOR_ASSIGNMENT;	\
	}

	#ifdef __cplusplus
		#define NEXT_CHAR yyinput
	#else
		#define NEXT_CHAR input
	#endif

	#define token_type int

	#include "syntax-tree-node.h"

	typedef union {
		operator_t operator;
		int number;
		char* string;
		struct syntax_node_t* node;
	} token_value_t;
	

	int yywrap();
	int fileno(FILE* file);

#define YYSTYPE token_value_t
	
	extern YYSTYPE yylval;
	size_t line, col;

	void flex_init();
	void flush();

	void readen_char();
	void readen_token();
	void new_line();

	int parse_bin(char* text);
	int parse_dec(char* text);
	int parse_hex(char* text);
	
	char parse_char(char* text);	
	char* string_const(char* text, size_t length);
	
	token_type multiline_comment(char* text);
	token_type oneline_comment(char* text);
	char* variable_name(char* text, size_t length);
	
	void invalid_token(char* text, int line, int col);
#endif
