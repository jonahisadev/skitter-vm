#include "include/bitmask.h"

void bitmask_set(unsigned int* bitmask, int index) {
	*bitmask |= 1 << index;
}

int bitmask_get(unsigned int bitmask, int index) {
	return (bitmask >> index) & 1;
}

void bitmask_clear(unsigned int* bitmask) {
	*bitmask = 0;
}