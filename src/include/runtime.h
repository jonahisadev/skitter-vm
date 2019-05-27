#ifndef SK_RUNTIME_H
#define SK_RUNTIME_H

#include "common.h"
#include "util.h"
#include "bytecode.h"
#include "bitmask.h"

struct Runtime_t {
	ByteBuffer* code;
	unsigned char stack[65536];
	unsigned int reg[BYTECODE_REGISTER_SIZE];
	unsigned int pc;
	unsigned int* sp;
	unsigned int cmp_flags;
};
typedef struct Runtime_t Runtime;

enum CompareIndex_t {
	COMPARE_EQUALS,
	COMPARE_GREATER_THAN,
	COMPARE_LESS_THAN,
	COMPARE_ZERO,
};
typedef enum CompareIndex_t CompareIndex;

Runtime* create_runtime(unsigned char* code, int len);
void runtime_stack_push_int(Runtime* runtime, unsigned int n);
void runtime_stack_push_byte(Runtime* runtime, unsigned char n);
unsigned int runtime_stack_pop_int(Runtime* runtime);
unsigned char runtime_stack_pop_byte(Runtime* runtime);
void runtime_compare(Runtime* runtime, unsigned int a, unsigned int b);
void delete_runtime(Runtime* runtime);

int run_bytecode(unsigned char* code, int len);

#endif // SK_RUNTIME_H