#include "include/compiler.h"

#define TOKEN(x) tokens[i + (x)]

CompilerStatus compile_tokens(TokenList* list, const char* filename) {
	int i = 0;
	Token* token = NULL;
	ByteBuffer* buffer = create_byte_buffer();

	while (i < list->ptr) {
		token = list->tokens[i];

		switch (token->data) {
			case (MOV): {
				if (list->TOKEN(1)->type == REGISTER && list->TOKEN(2)->type == NUM) {
					write_byte_to_buffer(buffer, BYTECODE_MOV_REG_CONST);
					write_byte_to_buffer(buffer, list->TOKEN(1)->data);
					write_int_to_buffer(buffer, list->TOKEN(2)->data);
				} else if (list->TOKEN(1)->type == REGISTER && list->TOKEN(2)->type == REGISTER) {
					write_byte_to_buffer(buffer, BYTECODE_MOV_REG_REG);
					write_byte_to_buffer(buffer, list->TOKEN(1)->data);
					write_byte_to_buffer(buffer, list->TOKEN(2)->data);
				}
				i += 3;
				break;
			}

			case (PUSH): {
				if (list->TOKEN(1)->type == NUM) {
					write_byte_to_buffer(buffer, BYTECODE_PUSH_CONST);
					write_int_to_buffer(buffer, list->TOKEN(1)->data);
				}
				i += 2;
				break;
			}

			case (ADD): {
				if (list->TOKEN(1)->type == INST) {
					write_byte_to_buffer(buffer, BYTECODE_ADD_STACK);
				} else if (list->TOKEN(1)->type == REGISTER && list->TOKEN(2)->type == REGISTER) {
					write_byte_to_buffer(buffer, BYTECODE_ADD_REG_REG);
					write_byte_to_buffer(buffer, list->TOKEN(1)->data);
					write_byte_to_buffer(buffer, list->TOKEN(2)->data);
				}
				i++;
				break;
			}

			case (SUB): {
				if (list->TOKEN(1)->type == INST) {
					write_byte_to_buffer(buffer, BYTECODE_SUB_STACK);
				}
				i++;
				break;
			}

			case (MUL): {
				if (list->TOKEN(1)->type == INST) {
					write_byte_to_buffer(buffer, BYTECODE_MUL_STACK);
				}
				i++;
				break;
			}

			case (HLT): {
				write_byte_to_buffer(buffer, BYTECODE_HLT);
				i++;
				break;
			}
		}
	}

	write_file_binary(filename, buffer);
	delete_byte_buffer(buffer);
	return COMPILER_STATUS_SUCCESS;
}