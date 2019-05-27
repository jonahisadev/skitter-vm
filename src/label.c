#include "include/label.h"

Label* create_label(char* id, unsigned int addr) {
	Label* label = (Label*) malloc(sizeof(Label));
	label->id = (char*) malloc(sizeof(char) * strlen(id));
	strcpy(label->id, id);
	label->addr = addr;
	return label;
}

void delete_label(Label* label) {
	free(label->id);
	free(label);
}

LabelList* create_label_list() {
	LabelList* list = (LabelList*) malloc(sizeof(LabelList));
	list->labels = (Label**) malloc(sizeof(Label*));
	list->ptr = 0;
	list->alloc_size = 1;
	return list;
}

void add_label_to_list(LabelList* list, Label* label) {
	if (list->ptr >= list->alloc_size) {
		list->alloc_size *= 2;
		list->labels = (Label**) realloc(list->labels, sizeof(Label*) * list->alloc_size);
	}

	list->labels[list->ptr++] = label;
}

int label_list_contains(LabelList* list, char* id) {
	for (int i = 0; i < list->ptr; i++) {
		if (strcmp(list->labels[i]->id, id) == 0)
			return i;
	}
	return -1;
}

void delete_label_list(LabelList* list) {
	for (int i = 0; i < list->ptr; i++) {
		delete_label(list->labels[i]);
	}
	free(list);
}