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
		// printf("(%d) %d: %d\n", i, token->type, token->data);

		if (token->type == INST) {
			switch (token->data) {
				case (MOV): {
					if (tokens->TOKEN(1)->type == REGISTER && tokens->TOKEN(2)->type == NUMBER) {
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
					if (tokens->TOKEN(1)->type == NUMBER) {
						write_byte_to_buffer(buffer, BYTECODE_PUSH_CONST);
						write_int_to_buffer(buffer, tokens->TOKEN(1)->data);
					}
					i += 2;
					break;
				}

				case (ADD): {
					if (tokens->TOKEN(1)->type == INST) {
						write_byte_to_buffer(buffer, BYTECODE_ADD_STACK);
						i++;
					} else if (tokens->TOKEN(1)->type == REGISTER && tokens->TOKEN(2)->type == REGISTER) {
						write_byte_to_buffer(buffer, BYTECODE_ADD_REG_REG);
						write_byte_to_buffer(buffer, tokens->TOKEN(1)->data);
						write_byte_to_buffer(buffer, tokens->TOKEN(2)->data);
						i += 3;
					}
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
						i++;
					} else if (tokens->TOKEN(1)->type == REGISTER && tokens->TOKEN(2)->type == REGISTER) {
						write_byte_to_buffer(buffer, BYTECODE_MUL_REG_REG);
						write_byte_to_buffer(buffer, tokens->TOKEN(1)->data);
						write_byte_to_buffer(buffer, tokens->TOKEN(2)->data);
						i += 3;
					}
					break;
				}

				case (INC): {
					if (tokens->TOKEN(1)-> type == REGISTER) {
						write_byte_to_buffer(buffer, BYTECODE_INC);
						write_byte_to_buffer(buffer, tokens->TOKEN(1)->data);
						i += 2;
					}
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

				case (CMP): {
					if (tokens->TOKEN(1)->type == REGISTER && tokens->TOKEN(2)->type == REGISTER) {
						write_byte_to_buffer(buffer, BYTECODE_CMP_REG_REG);
						write_byte_to_buffer(buffer, tokens->TOKEN(1)->data);
						write_byte_to_buffer(buffer, tokens->TOKEN(2)->data);
					} else if (tokens->TOKEN(1)->type == REGISTER && tokens->TOKEN(2)->type == NUMBER) {
						write_byte_to_buffer(buffer, BYTECODE_CMP_REG_NUM);
						write_byte_to_buffer(buffer, tokens->TOKEN(1)->data);
						write_int_to_buffer(buffer, tokens->TOKEN(2)->data);
					}
					i += 3;
					break;
				}

				case (JE): {
					if (tokens->TOKEN(1)->type == JUMP_TO) {
						write_byte_to_buffer(buffer, BYTECODE_JE);
					}
					i++;
					break;
				}

				case (JNE): {
					if (tokens->TOKEN(1)->type == JUMP_TO) {
						write_byte_to_buffer(buffer, BYTECODE_JNE);
					}
					i++;
					break;
				}

				case (JGT): {
					if (tokens->TOKEN(1)->type == JUMP_TO) {
						write_byte_to_buffer(buffer, BYTECODE_JGT);
					}
					i++;
					break;
				}

				case (JGE): {
					if (tokens->TOKEN(1)->type == JUMP_TO) {
						write_byte_to_buffer(buffer, BYTECODE_JGE);
					}
					i++;
					break;
				}

				case (JLT): {
					if (tokens->TOKEN(1)->type == JUMP_TO) {
						write_byte_to_buffer(buffer, BYTECODE_JLT);
					}
					i++;
					break;
				}

				case (JLE): {
					if (tokens->TOKEN(1)->type == JUMP_TO) {
						write_byte_to_buffer(buffer, BYTECODE_JLE);
					}
					i++;
					break;
				}

				case (JZ): {
					if (tokens->TOKEN(1)->type == JUMP_TO) {
						write_byte_to_buffer(buffer, BYTECODE_JZ);
					}
					i++;
					break;
				}

				case (JNZ): {
					if (tokens->TOKEN(1)->type == JUMP_TO) {
						write_byte_to_buffer(buffer, BYTECODE_JNZ);
					}
					i++;
					break;
				}

				case (CALL): {
					if (tokens->TOKEN(1)->type == JUMP_TO) {
						write_byte_to_buffer(buffer, BYTECODE_CALL);
					}
					i++;
					break;
				}

				case (RET): {
					write_byte_to_buffer(buffer, BYTECODE_RET);
					i++;
					break;
				}

				case (HLT): {
					write_byte_to_buffer(buffer, BYTECODE_HLT);
					i++;
					break;
				}

				case (HELLO): {
					write_byte_to_buffer(buffer, BYTECODE_HELLO);
					i++;
					break;
				}
			}
		} else if (token->type == LABEL) {
			Label* label = cs->labels->labels[token->data];
			label->addr = buffer->length;
			i++;
		} else if (token->type == JUMP_TO) {
			cs->jumps->labels[token->data]->addr = buffer->length;
			write_int_to_buffer(buffer, 0);
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