#ifndef SK_BITMASK_H
#define SK_BITMASK_H

void bitmask_set(unsigned int* bitmask, int index);
int bitmask_get(unsigned int bitmask, int index);
void bitmask_clear(unsigned int* bitmask);

#endif // SK_BITMASK_H