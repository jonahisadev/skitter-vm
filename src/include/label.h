#ifndef SK_LABEL_H
#define SK_LABEL_H

#include "common.h"

struct Label_t {
	char* id;
	unsigned int addr;
};
typedef struct Label_t Label;

Label* create_label(char* id, unsigned int addr);
void delete_label(Label* label);

struct LabelList_t {
	Label** labels;
	int ptr;
	int alloc_size;
};
typedef struct LabelList_t LabelList;

LabelList* create_label_list();
void add_label_to_list(LabelList* list, Label* label);
int label_list_contains(LabelList* list, char* id);
void delete_label_list(LabelList* list);

#endif // SK_LABEL_H