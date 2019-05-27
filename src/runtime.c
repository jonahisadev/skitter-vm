#include "include/runtime.h"

#define CONDITIONAL_JUMP(flag, on) \
	if (bitmask_get(runtime->cmp_flags, (flag)) == on) { \
		runtime->pc = addr; \
	} else { \
		runtime->pc += CONSTANT_SIZE_INT; \
	}

#define CONDITIONAL_JUMP_2(flag, flag2, on) \
	if (bitmask_get(runtime->cmp_flags, (flag)) == on || bitmask_get(runtime->cmp_flags, (flag2)) == on) { \
		runtime->pc = addr; \
	} else { \
		runtime->pc += CONSTANT_SIZE_INT; \
	}

Runtime* create_runtime(unsigned char* code, int len) {
	Runtime* runtime = (Runtime*) malloc(sizeof(Runtime));
	runtime->code = create_byte_buffer();
	load_byte_buffer(runtime->code, code, len);
	runtime->pc = 0;
	runtime->sp = &runtime->reg[BYTECODE_REGISTER_SP];
	bitmask_clear(&runtime->cmp_flags);
	return runtime;
}

void runtime_stack_push_int(Runtime* runtime, unsigned int n) {
	runtime_stack_push_byte(runtime, n & 0xFF000000);
	runtime_stack_push_byte(runtime, n & 0x00FF0000);
	runtime_stack_push_byte(runtime, n & 0x0000FF00);
	runtime_stack_push_byte(runtime, n & 0x000000FF);
}

void runtime_stack_push_byte(Runtime* runtime, unsigned char n) {
	runtime->stack[++(*runtime->sp)] = n;
}

unsigned int runtime_stack_pop_int(Runtime* runtime) {
	unsigned char a = runtime_stack_pop_byte(runtime);
	unsigned char b = runtime_stack_pop_byte(runtime);
	unsigned char c = runtime_stack_pop_byte(runtime);
	unsigned char d = runtime_stack_pop_byte(runtime);
	return (d << 24) | (c << 16) | (b << 8) | a;
}

unsigned char runtime_stack_pop_byte(Runtime* runtime) {
	return runtime->stack[(*runtime->sp)--];
}

void runtime_compare(Runtime* runtime, unsigned int a, unsigned int b) {
	bitmask_clear(&runtime->cmp_flags);
	if (a == b) {
		bitmask_set(&runtime->cmp_flags, COMPARE_EQUALS);
		bitmask_set(&runtime->cmp_flags, COMPARE_ZERO);
	}
	if (a < b)
		bitmask_set(&runtime->cmp_flags, COMPARE_LESS_THAN);
	if (a > b)
		bitmask_set(&runtime->cmp_flags, COMPARE_GREATER_THAN);
}

void delete_runtime(Runtime* runtime) {
	delete_byte_buffer(runtime->code);
	free(runtime);
}

int run_bytecode(unsigned char* n_code, int len) {
	Runtime* runtime = create_runtime(n_code, len);
	unsigned char* code = runtime->code->buffer;
	int status = 0;
	bool running = true;

	while (running) {
		switch (code[runtime->pc++]) {

			// MOV %XX #N
			case (BYTECODE_MOV_REG_CONST): {
				int reg = read_byte_from_buffer(runtime->code, runtime->pc++);
				unsigned int num = read_int_from_buffer(runtime->code, runtime->pc);
				runtime->reg[reg] = num;
				runtime->pc += CONSTANT_SIZE_INT;
				break;
			}

			// MOV %XX %YY
			case (BYTECODE_MOV_REG_REG): {
				int a = read_byte_from_buffer(runtime->code, runtime->pc++);
				int b = read_byte_from_buffer(runtime->code, runtime->pc++);
				runtime->reg[a] = runtime->reg[b];
				break;
			}

			// PUSH #N
			case (BYTECODE_PUSH_CONST): {
				runtime_stack_push_int(runtime, read_int_from_buffer(runtime->code, runtime->pc));
				runtime->pc += CONSTANT_SIZE_INT;
				break;
			}

			// ADD
			case (BYTECODE_ADD_STACK): {
				unsigned int a = runtime_stack_pop_int(runtime);
				unsigned int b = runtime_stack_pop_int(runtime);
				runtime_stack_push_int(runtime, a + b);
				break;
			}

			// ADD %XX %YY
			case (BYTECODE_ADD_REG_REG): {
				unsigned int a = read_byte_from_buffer(runtime->code, runtime->pc++);
				unsigned int b = read_byte_from_buffer(runtime->code, runtime->pc++);
				runtime->reg[a] += runtime->reg[b];
				break;
			}

			// SUB
			case (BYTECODE_SUB_STACK): {
				unsigned int a = runtime_stack_pop_int(runtime);
				unsigned int b = runtime_stack_pop_int(runtime);
				runtime_stack_push_int(runtime, b - a);
				break;
			}

			// MUL %XX %YY
			case (BYTECODE_MUL_REG_REG): {
				unsigned int a = read_byte_from_buffer(runtime->code, runtime->pc++);
				unsigned int b = read_byte_from_buffer(runtime->code, runtime->pc++);
				runtime->reg[a] *= runtime->reg[b];
				break;
			}

			// MUL
			case (BYTECODE_MUL_STACK): {
				unsigned int a = runtime_stack_pop_int(runtime);
				unsigned int b = runtime_stack_pop_int(runtime);
				runtime_stack_push_int(runtime, a * b);
				break;
			}

			// INC %XX
			case (BYTECODE_INC): {
				unsigned int reg = read_byte_from_buffer(runtime->code, runtime->pc++);
				runtime->reg[reg]++;
				break;
			}

			// JMP @
			case (BYTECODE_JMP): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				runtime->pc = addr;
				break;
			}

			// CMP %XX #N
			case (BYTECODE_CMP_REG_NUM): {
				unsigned int reg = read_byte_from_buffer(runtime->code, runtime->pc++);
				unsigned int num = read_int_from_buffer(runtime->code, runtime->pc);
				runtime->pc += CONSTANT_SIZE_INT;
				runtime_compare(runtime, runtime->reg[reg], num);
				break;
			}

			// JE @
			case (BYTECODE_JE): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				CONDITIONAL_JUMP(COMPARE_EQUALS, 1)
				break;
			}

			// JNE @
			case (BYTECODE_JNE): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				CONDITIONAL_JUMP(COMPARE_EQUALS, 0)
				break;
			}

			// JGT @
			case (BYTECODE_JGT): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				CONDITIONAL_JUMP(COMPARE_GREATER_THAN, 1)
				break;
			}

			// JGE @
			case (BYTECODE_JGE): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				CONDITIONAL_JUMP_2(COMPARE_GREATER_THAN, COMPARE_EQUALS, 1)
				break;
			}

			// JLT @
			case (BYTECODE_JLT): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				CONDITIONAL_JUMP(COMPARE_LESS_THAN, 1)
				break;
			}

			// JLE @
			case (BYTECODE_JLE): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				CONDITIONAL_JUMP_2(COMPARE_LESS_THAN, COMPARE_EQUALS, 1)
				break;
			}

			// JZ @
			case (BYTECODE_JZ): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				CONDITIONAL_JUMP(COMPARE_ZERO, 1)
				break;
			}

			// JNZ @
			case (BYTECODE_JNZ): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				CONDITIONAL_JUMP(COMPARE_ZERO, 0);
				break;
			}

			// CALL @
			case (BYTECODE_CALL): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				runtime->pc += CONSTANT_SIZE_INT;

				runtime->reg[BYTECODE_REGISTER_BP] = *runtime->sp;
				runtime_stack_push_int(runtime, runtime->pc);

				runtime->pc = addr;
				break;
			}

			// RET
			case (BYTECODE_RET): {
				unsigned int addr = runtime_stack_pop_int(runtime);
				runtime->pc = addr;
				break;
			}

			// HLT
			case (BYTECODE_HLT): {
				status = runtime->reg[BYTECODE_REGISTER_BX];
				running = false;
				break;
			}

			case (BYTECODE_HELLO): {
				printf("Hello!\n");
				break;
			}
		}
	}

	return status;
}