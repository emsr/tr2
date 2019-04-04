// gcc -std=gnu11 static_assert.c

#include <assert.h> // For C++ish static_assert

_Static_assert( sizeof(int) == sizeof(long int), "Error!");

static_assert( sizeof(int) == sizeof(long int), "Error!");
