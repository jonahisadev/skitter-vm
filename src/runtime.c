#include "include/runtime.h"

Runtime* create_runtime(unsigned char* code, int len) {
	Runtime* runtime = (Runtime*) malloc(sizeof(Runtime));
	runtime->code = create_byte_buffer();
	load_byte_buffer(runtime->code, code, len);
	runtime->pc = 0;
	runtime->sp = &runtime->reg[BYTECODE_REGISTER_SP];
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

			// MOV $XX #N
			case (BYTECODE_MOV_REG_CONST): {
				int reg = read_byte_from_buffer(runtime->code, runtime->pc++);
				unsigned int num = read_int_from_buffer(runtime->code, runtime->pc);
				runtime->reg[reg] = num;
				runtime->pc += CONSTANT_SIZE_INT;
				break;
			}

			// MOV $XX $YY
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

			// ADD $XX $YY
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

			// MUL
			case (BYTECODE_MUL_STACK): {
				unsigned int a = runtime_stack_pop_int(runtime);
				unsigned int b = runtime_stack_pop_int(runtime);
				runtime_stack_push_int(runtime, a * b);
				break;
			}

			case (BYTECODE_JMP): {
				unsigned int addr = read_int_from_buffer(runtime->code, runtime->pc);
				runtime->pc = addr;
				break;
			}

			// HLT
			case (BYTECODE_HLT): {
				status = runtime->reg[BYTECODE_REGISTER_BX];
				running = false;
				break;
			}
		}
	}

	return status;
}