#ifndef SK_COMMON_H
#define SK_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "list.h"

// Panic exit with message
#define panic(x, ...) fprintf(stderr, "%s:%d ", __FILE__, __LINE__); fprintf(stderr, (x), __VA_ARGS__); exit(1)

// Runtime equality assert
#define assert(x, y, ...) if (!(x)) panic((y), __VA_ARGS__)

#endif // SK_COMON_H