#ifndef SK_TOKENS_H
#define SK_TOKENS_H

#include "common.h"

enum TokenType_t {
	INST,
	NUMBER,
	REGISTER,
	LABEL,
	JUMP_TO,
};
typedef enum TokenType_t TokenType;

enum TokenInst_t {
	NO_INST,
	MOV,
	PUSH,
	POP,
	ADD,
	SUB,
	MUL,
	INC,
	JMP,
	CMP,
	JE,
	JNE,
	JGT,
	JGE,
	JLT,
	JLE,
	JZ,
	JNZ,
	CALL,
	RET,
	HLT,

	HELLO,
};
typedef enum TokenInst_t TokenInst;

enum TokenRegister_t {
	NO_REGISTER = 0,
	REGISTER_A = 1,
	REGISTER_B = 2,
	REGISTER_C = 3,
	REGISTER_D = 4,
	REGISTER_R = 5
};
typedef enum TokenRegister_t TokenRegister;

struct Token_t {
	TokenType type;
	int data;
	int line;
};
typedef struct Token_t Token;

struct TokenList_t {
	Token** tokens;
	int ptr;
	int size;
};
typedef struct TokenList_t TokenList;

Token* create_token(TokenType type, int data, int line);

TokenList* create_token_list();
void add_token_to_list(TokenList* list, Token* token);
Token* get_token_from_list(TokenList* list, int index);
void delete_token_list(TokenList* list);

#endif // SK_TOKENS_H