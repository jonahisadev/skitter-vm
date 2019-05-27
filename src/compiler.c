#include "include/compiler.h"

#define TOKEN(x) tokens[i + (x)]

CompilerState* create_compiler_state() {
	CompilerState* cs = malloc(sizeof(CompilerState));
	cs->status = 0;
	cs->tokens = create_token_list();
	cs->labels = create_label_list();
	cs->jumps = create_label_list();
	return cs;
}

void delete_compiler_state(CompilerState* cs) {
	free(cs->tokens);
	free(cs->labels);
	free(cs->jumps);
	free(cs);
}

CompilerStatus compile_tokens(CompilerState* cs, const char* filename) {
	int i = 0;
	TokenList* tokens = cs->tokens;
	Token* token = NULL;
	ByteBuffer* buffer = create_byte_buffer();

	while (i < tokens->ptr) {
		token = tokens->tokens[i];

		if (token->type == INST) {
			switch (token->data) {
				case (MOV): {
					if (tokens->TOKEN(1)->type == REGISTER && tokens->TOKEN(2)->type == NUM) {
						write_byte_to_buffer(buffer, BYTECODE_MOV_REG_CONST);
						write_byte_to_buffer(buffer, tokens->TOKEN(1)->data);
						write_int_to_buffer(buffer, tokens->TOKEN(2)->data);
					} else if (tokens->TOKEN(1)->type == REGISTER && tokens->TOKEN(2)->type == REGISTER) {
						write_byte_to_buffer(buffer, BYTECODE_MOV_REG_REG);
						write_byte_to_buffer(buffer, tokens->TOKEN(1)->data);
						write_byte_to_buffer(buffer, tokens->TOKEN(2)->data);
					}
					i += 3;
					break;
				}

				case (PUSH): {
					if (tokens->TOKEN(1)->type == NUM) {
						write_byte_to_buffer(buffer, BYTECODE_PUSH_CONST);
						write_int_to_buffer(buffer, tokens->TOKEN(1)->data);
					}
					i += 2;
					break;
				}

				case (ADD): {
					if (tokens->TOKEN(1)->type == INST) {
						write_byte_to_buffer(buffer, BYTECODE_ADD_STACK);
					} else if (tokens->TOKEN(1)->type == REGISTER && tokens->TOKEN(2)->type == REGISTER) {
						write_byte_to_buffer(buffer, BYTECODE_ADD_REG_REG);
						write_byte_to_buffer(buffer, tokens->TOKEN(1)->data);
						write_byte_to_buffer(buffer, tokens->TOKEN(2)->data);
					}
					i++;
					break;
				}

				case (SUB): {
					if (tokens->TOKEN(1)->type == INST) {
						write_byte_to_buffer(buffer, BYTECODE_SUB_STACK);
					}
					i++;
					break;
				}

				case (MUL): {
					if (tokens->TOKEN(1)->type == INST) {
						write_byte_to_buffer(buffer, BYTECODE_MUL_STACK);
					}
					i++;
					break;
				}

				case (JMP): {
					if (tokens->TOKEN(1)->type == JUMP_TO) {
						write_byte_to_buffer(buffer, BYTECODE_JMP);
						cs->jumps->labels[tokens->TOKEN(1)->data]->addr = buffer->length;
						write_int_to_buffer(buffer, 0);
					}
					i += 2;
					break;
				}

				case (HLT): {
					write_byte_to_buffer(buffer, BYTECODE_HLT);
					i++;
					break;
				}
			}
		} else if (token->type == LABEL) {
			Label* label = cs->labels->labels[token->data];
			label->addr = buffer->length;
			i++;
		}
	}

	// Fill in jump addresses
	for (i = 0; i < cs->jumps->ptr; i++) {
		Label* jmp = cs->jumps->labels[i];
		int label_index;
		if ((label_index = label_list_contains(cs->labels, jmp->id)) != -1) {
			write_int_at_buffer_loc(buffer, jmp->addr, cs->labels->labels[label_index]->addr);
		} else {
			printf("No such label '%s'\n", jmp->id);
		}
	}

	write_file_binary(filename, buffer);
	delete_byte_buffer(buffer);
	return COMPILER_STATUS_SUCCESS;
}