#ifndef SK_RUNTIME_H
#define SK_RUNTIME_H

#include "common.h"
#include "util.h"
#include "bytecode.h"

struct Runtime_t {
	ByteBuffer* code;
	unsigned char stack[65536];
	unsigned int reg[BYTECODE_REGISTER_SIZE];
	unsigned int pc;
	unsigned int* sp;
};
typedef struct Runtime_t Runtime;

Runtime* create_runtime(unsigned char* code, int len);
void runtime_stack_push_int(Runtime* runtime, unsigned int n);
void runtime_stack_push_byte(Runtime* runtime, unsigned char n);
unsigned int runtime_stack_pop_int(Runtime* runtime);
unsigned char runtime_stack_pop_byte(Runtime* runtime);
void delete_runtime(Runtime* runtime);

int run_bytecode(unsigned char* code, int len);

#endif // SK_RUNTIME_H