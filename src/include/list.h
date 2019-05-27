#ifndef SK_LIST_H
#define SK_LIST_H

#include "common.h"

struct StringList_t {
	char** data;
	int ptr;
	int alloc_size;
};
typedef struct StringList_t StringList;

StringList* create_string_list();
void add_string_to_list(StringList* list, char* str);
void delete_string_list(StringList* list);

struct UIntList_t {
	unsigned int* data;
	int ptr;
	int alloc_size;
};
typedef struct UIntList_t UIntList;

UIntList* create_uint_list();
void add_uint_to_list(UIntList* list, int num);
void delete_uint_list(UIntList* list);

#endif // SK_LIST_H