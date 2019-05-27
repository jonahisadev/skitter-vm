#include "include/list.h"

StringList* create_string_list() {
	StringList* list = (StringList*) malloc(sizeof(StringList));
	list->data = (char**) malloc(sizeof(char*));
	list->ptr = 0;
	list->alloc_size = 1;
	return list;
}

void add_string_to_list(StringList* list, char* str) {
	if (list->ptr >= list->alloc_size) {
		list->alloc_size *= 2;
		list->data = (char**) realloc(list->data, list->alloc_size * sizeof(char*));
	}

	list->data[list->ptr] = (char*) malloc(sizeof(char) * strlen(str));
	strcpy(list->data[list->ptr++], str);
}

void delete_string_list(StringList* list) {
	for (int i = 0; i < list->ptr; i++) {
		free(list->data[i]);
	}
	free(list->data);
	free(list);
}