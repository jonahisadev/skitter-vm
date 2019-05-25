#include "include/tokens.h"

Token* create_token(TokenType type, int data, int line) {
	Token* token = (Token*) malloc(sizeof(Token));
	token->type = type;
	token->data = data;
	token->line = line;
	return token;
}

TokenList* create_token_list() {
	TokenList* list = (TokenList*) malloc(sizeof(TokenList));
	list->tokens = malloc(sizeof(Token*));
	list->ptr = 0;
	list->size = 1;
	return list;
}

void add_token_to_list(TokenList* list, Token* token) {
	if (list->ptr >= list->size) {
		list->size *= 2;
		list->tokens = (Token**) realloc(list->tokens, sizeof(Token*) * list->size);
	}

	list->tokens[list->ptr++] = token;
}

Token* get_token_from_list(TokenList* list, int index) {
	return list->tokens[index];
}

void delete_token_list(TokenList* list) {
	for (int i = 0; i < list->ptr; i++) {
		free(list->tokens[i]);
	}
	free(list);
}