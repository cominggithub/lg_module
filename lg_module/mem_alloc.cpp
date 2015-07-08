#include "mem_alloc.h"
#include <stdlib.h>

void* mem_alloc(uint32_t size)
{
    // void* d;
    // d = malloc(size);
    // memset(d, 0, size);
    // return d;

    return calloc(1, size);
}

void mem_free(void **ptr)
{
    free(*ptr);
    *ptr = NULL;
}