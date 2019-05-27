#ifndef SK_COMPILER_H
#define SK_COMPILER_H

#include "common.h"
#include "util.h"
#include "tokens.h"
#include "bytecode.h"
#include "label.h"

enum CompilerStatus_t {
	COMPILER_STATUS_SUCCESS,
	COMPILER_SYNTAX_ERROR
};
typedef enum CompilerStatus_t CompilerStatus;

struct CompilerState_t {
	CompilerStatus status;
	TokenList* tokens;
	LabelList* labels;
	LabelList* jumps;
};
typedef struct CompilerState_t CompilerState;

CompilerState* create_compiler_state();
void delete_compiler_state(CompilerState* cs);

CompilerStatus compile_tokens(CompilerState* cs, const char* filename);

#endif // SK_COMPILER_H