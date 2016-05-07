#ifndef _AST_H_
#define _AST_H_

#include "tokens.h"

union ast_node_value_t;
struct ast_node_t;

typedef union ast_node_value_t {
	long number;
	char* string;
	int operator;
	int keyword;
	struct ast_node_t* child;
	//TODO
} ast_node_value_t;



typedef struct ast_node_t {
	TOKEN_TYPE_T type;
	union ast_node_value_t value;
	struct ast_node_t* next;
} ast_node_t;


ast_node_t* create_new_node();

ast_node_t* create_number(long value);
ast_node_t* create_identifier(char* name);


ast_node_t* create_unary(TOKEN_TYPE_T operator, ast_node_t* expr);
ast_node_t* create_binary(TOKEN_TYPE_T operator, ast_node_t* expr1, ast_node_t* expr2);
ast_node_t* create_ternary(TOKEN_TYPE_T operator, ast_node_t* expr1, ast_node_t* expr2, ast_node_t* expr3);


ast_node_t* create_list1(TOKEN_TYPE_T type, ast_node_t* expr1);
ast_node_t* create_list2(TOKEN_TYPE_T type, ast_node_t* expr1, ast_node_t* expr2);
ast_node_t* create_list3(TOKEN_TYPE_T type, ast_node_t* expr1, ast_node_t* expr2, ast_node_t* expr3);
ast_node_t* create_list4(TOKEN_TYPE_T type, ast_node_t* expr1, ast_node_t* expr2, ast_node_t* expr3, ast_node_t* expr4);


/*
 typedef int operator_t;

 typedef enum syntax_node_type {
 NUMBER,
 TEXT,
 VARIABLE,
 OPERATION,
 ASSIGNMENT,
 COMMANDS,
 SPECIAL
 } syntax_node_type;


 typedef enum special_t {
 IF,
 FOR,
 WHILE,
 DO_WHILE,
 BREAK,
 CONTINUE,
 PRINT,
 SCAN
 } special_t;

 typedef union syntax_node_value {
 int number;
 char* text;
 operator_t operator;
 special_t special;
 } syntax_node_value;


 typedef struct syntax_node_t {
 syntax_node_type type;
 union syntax_node_value value;
 struct syntax_node_t* next;
 struct syntax_node_t* child;
 } syntax_node_t;


 syntax_node_t* create_number(int number);
 syntax_node_t* create_text(char* text);

 syntax_node_t* create_assignment(operator_t asg_operator, syntax_node_t* variable, syntax_node_t* expr);
 syntax_node_t* create_unary(operator_t operator, syntax_node_t* expr);
 syntax_node_t* create_binary(operator_t operator, syntax_node_t* expr1, syntax_node_t* expr2);

 syntax_node_t* create_print(syntax_node_t* string, syntax_node_t* expr);
 syntax_node_t* create_scan(syntax_node_t* var);
 syntax_node_t* create_if(syntax_node_t* cond, syntax_node_t* cmd, syntax_node_t* cmdelse);
 syntax_node_t* create_for(syntax_node_t* init, syntax_node_t* cond, syntax_node_t* incr, syntax_node_t* cmd);
 syntax_node_t* create_do_while(syntax_node_t* cond, syntax_node_t* cmd);
 syntax_node_t* create_while(syntax_node_t* cond, syntax_node_t* cmd);
 syntax_node_t* create_keyword(special_t keyword);
 syntax_node_t* create_commands(syntax_node_t* command, syntax_node_t* commands);
 syntax_node_t* create_empty_command();

 syntax_node_t* create_list1(syntax_node_type type, syntax_node_t* expr1);
 syntax_node_t* create_list2(syntax_node_type type, syntax_node_t* expr1, syntax_node_t* expr2);
 syntax_node_t* create_list3(syntax_node_type type, syntax_node_t* expr1, syntax_node_t* expr2, syntax_node_t* expr3);
 syntax_node_t* create_list4(syntax_node_type type, syntax_node_t* expr1, syntax_node_t* expr2, syntax_node_t* expr3, syntax_node_t* expr4);

 void print_root(syntax_node_t* root);
 void print_padding(int padding);
 void print_node(int padding, syntax_node_t* node);
 void print_node_label(syntax_node_t* node);
 void print_special(int padding, syntax_node_t* node);
 */
#endif

