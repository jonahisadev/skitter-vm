#include "include/lexer.h"

CompilerState* run_lexer(char* source) {
	CompilerState* cs = create_compiler_state();
	TokenList* list = cs->tokens;
	char lex[256];
	int lexi = 0;
	int i = 0;
	int line = 1;

	while (true) {
		memset(lex, '\0', 256);

		while (!is_lexer_char_important(source[i])) {
			lex[lexi++] = source[i++];
		}

		// Skip over tabs
		if (lexi == 0 && source[i] == '\t') {
			i++;
			lexi = 0;
			continue;
		}

		// Skip empty lines
		if (lexi == 0 && source[i] == '\n') {
			i++;
			lexi = 0;
			line++;
			continue;
		}

		// Number
		if (lex[0] == '#') {
			add_token_to_list(list, create_token(NUMBER, convert_to_number(lex), line));
		}

		// Register
		else if (lex[0] == '%') {
			TokenRegister reg = lexer_get_register(lex);
			if (reg == NO_REGISTER) {
				printf("REG ERROR: %d, %s\n", reg, lex);
			}
			add_token_to_list(list, create_token(REGISTER, reg, line));
		}

		// Label
		else if (lex[strlen(lex)-1] == ':') {
			lex[strlen(lex)-1] = '\0';
			if (label_list_contains(cs->labels, lex) != -1) {
				printf("Duplicate label '%s' at line %d\n", lex, line);
				cs->status = COMPILER_SYNTAX_ERROR;
				goto advance;
			}
			add_token_to_list(list, create_token(LABEL, cs->labels->ptr, line));
			add_label_to_list(cs->labels, create_label(lex, 0));
		}

		// Jump To
		else if (lex[0] == '@') {
			char id[strlen(lex)];
			memcpy(id, &lex[1], strlen(lex));
			id[strlen(lex)-1] = '\0';
			add_token_to_list(list, create_token(JUMP_TO, cs->jumps->ptr, line));
			add_label_to_list(cs->jumps, create_label(id, 0));
		}

		// Instruction
		else {
			TokenInst inst = lexer_get_instruction(lex);
			// printf("INST: %d (%s)\n", inst, lex);
			if (inst == NO_INST) {
				printf("INST ERROR: %d, %s\n", inst, lex);
			}
			add_token_to_list(list, create_token(INST, inst, line));
		}

		advance:
		if (source[i] == '\0')
			break;
		if (source[i] == '\n')
			line++;
		i++;
		lexi = 0;
	}

	return cs;
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
	if (strcmp(str, "inc") == 0)
		return INC;
	if (strcmp(str, "jmp") == 0)
		return JMP;
	if (strcmp(str, "cmp") == 0)
		return CMP;
	if (strcmp(str, "je") == 0)
		return JE;
	if (strcmp(str, "jne") == 0)
		return JNE;
	if (strcmp(str, "jgt") == 0)
		return JGT;
	if (strcmp(str, "jge") == 0)
		return JGE;
	if (strcmp(str, "jlt") == 0)
		return JLT;
	if (strcmp(str, "jle") == 0)
		return JLE;
	if (strcmp(str, "jz") == 0)
		return JZ;
	if (strcmp(str, "jnz") == 0)
		return JNZ;
	if (strcmp(str, "call") == 0)
		return CALL;
	if (strcmp(str, "ret") == 0)
		return RET;
	if (strcmp(str, "hlt") == 0)
		return HLT;
	if (strcmp(str, "hello") == 0)
		return HELLO;
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