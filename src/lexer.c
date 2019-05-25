#include "include/lexer.h"

TokenList* run_lexer(char* source) {
	TokenList* list = create_token_list();
	char lex[256];
	int lexi = 0;
	int i = 0;
	int line = 1;

	while (true) {
		memset(lex, '\0', 256);

		while (!is_lexer_char_important(source[i])) {
			lex[lexi++] = source[i++];
		}

		// Number
		if (lex[0] == '#') {
			add_token_to_list(list, create_token(NUM, convert_to_number(lex), line));
		}

		// Register
		else if (lex[0] == '%') {
			TokenRegister reg = lexer_get_register(lex);
			if (reg == NO_REGISTER) {
				printf("REG ERROR: %d, %s\n", reg, lex);
			}
			add_token_to_list(list, create_token(REGISTER, reg, line));
		}

		// Instruction
		else {
			TokenInst inst = lexer_get_instruction(lex);
			// printf("INST: %d (%s)\n", inst, lex);
			if (inst == NO_INST) {
				printf("ERROR: %d, %s\n", inst, lex);
			}
			add_token_to_list(list, create_token(INST, inst, line));
		}

		if (source[i] == '\0')
			break;
		if (source[i] == '\n')
			line++;
		i++;
		lexi = 0;
	}

	return list;
}

bool is_lexer_char_important(char c) {
	return
		c == ' ' ||
		c == '\n' ||
		c == '\t' ||
		c == '\0'
	;
}

int lexer_get_instruction(char* str) {
	if (strcmp(str, "mov") == 0)
		return MOV;
	if (strcmp(str, "push") == 0)
		return PUSH;
	if (strcmp(str, "pop") == 0)
		return POP;
	if (strcmp(str, "add") == 0)
		return ADD;
	if (strcmp(str, "sub") == 0)
		return SUB;
	if (strcmp(str, "mul") == 0)
		return MUL;
	if (strcmp(str, "hlt") == 0)
		return HLT;
	return NO_INST;
}

int lexer_get_register(char* str) {
	if (strcmp(str, "%ax") == 0)
		return REGISTER_A;
	if (strcmp(str, "%bx") == 0)
		return REGISTER_B;
	if (strcmp(str, "%cx") == 0)
		return REGISTER_C;
	if (strcmp(str, "%dx") == 0)
		return REGISTER_D;
	if (strcmp(str, "%rx") == 0)
		return REGISTER_R;
	return NO_REGISTER;
}

int convert_to_number(char* num) {
	int len = strlen(num);
	char n[len-1];
	for (int i = 0; i < len - 1; i++) {
		n[i] = num[i+1];
	}
	return atoi(n);
}