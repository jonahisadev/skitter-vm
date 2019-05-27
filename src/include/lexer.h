#ifndef SK_LEXER_H
#define SK_LEXER_H

#include "common.h"
#include "tokens.h"
#include "compiler.h"

CompilerState* run_lexer(char* source);
bool is_lexer_char_important(char c);
int lexer_get_instruction(char* str);
int lexer_get_register(char* str);
int convert_to_number(char* num);

#endif // SK_LEXER_H