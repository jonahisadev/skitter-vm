#include "include/common.h"
#include "include/util.h"
#include "include/tokens.h"
#include "include/lexer.h"
#include "include/bytecode.h"
#include "include/compiler.h"
#include "include/runtime.h"

void print_help() {
	printf("Skitter VM v0.0 - Help\n\n");
	printf("Options:\n");
	printf("    compile [file] [output]: Compile instructions to bytecode\n");
	printf("    run [file]: Run bytecode in the virtual machine\n");
	printf("\nHope this helps!\n");
}

int main(int argc, char** argv) {
	if (argc < 3) {
		print_help();
		return 1;
	}

	// Compile
	if (strcmp(argv[1], "compile") == 0) {
		long prev = current_millis();

		const char* filename = argv[2];
		char* source = read_file_ascii(filename);
		CompilerState* cs = run_lexer(source);
		CompilerStatus status = compile_tokens(cs, argv[3]);
		if (status != COMPILER_STATUS_SUCCESS) {
			panic("Compiler Error: %d\n", status);
		}
		delete_compiler_state(cs);
		free(source);

		long now = current_millis() - prev;

		printf("Compilation time: %04ldms\n", now);
		return status;
	}

	// Run
	if (strcmp(argv[1], "run") == 0) {
		const char* filename = argv[2];
		int len = 0;
		unsigned char* code = read_file_binary(filename, &len);
		int status = run_bytecode(code, len);
		free(code);

		return status;
	}

	return 0;
}