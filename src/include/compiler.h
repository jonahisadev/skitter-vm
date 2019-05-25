#ifndef SK_COMPILER_H
#define SK_COMPILER_H

#include "util.h"
#include "tokens.h"
#include "bytecode.h"

enum CompilerStatus_t {
	COMPILER_STATUS_SUCCESS
};
typedef enum CompilerStatus_t CompilerStatus;

CompilerStatus compile_tokens(TokenList* list, const char* filename);

#endif // SK_COMPILER_H