#ifndef _AST_STACKODE_EXPORTER_H_
#define _AST_STACKODE_EXPORTER_H_

#include <stdio.h>
#include "ast-exporter.h"
#include "stackode.h"
#include "tokens.h"

void ast_export_root(FILE* dest, struct ast_node_t* root);
void export_stackode(FILE* dest, sk_program_t *program);

void export_sk_header(FILE* dest);
void export_sk_footer(FILE* dest);
void export_sk_instruction(FILE* dest, sk_instruction_t *instruction);


void print_sk_instr(FILE* dest, char* name);
void print_sk_instr_str(FILE* dest, char* name, char* str);
void print_sk_instr_num(FILE* dest, char* name, long num);
void print_sk_instr_oper(FILE* dest, char* name, TOKEN_TYPE_T oper);
void print_sk_instr_unquot_num(FILE* dest, char* name, char* arg1, long arg2);

#endif
