#ifndef _MEM_ALLOC_H_
#define _MEM_ALLOC_H_

#include <stdint.h>


#define ALLOC_BY_TYPE(t)               (t*)mem_alloc(sizeof(t))
#define ALLOC_BY_TYPE_AND_COUNT(t, c)  (t*)mem_alloc(sizeof(t)*c)
#define ALLOC_BY_SIZE(s)               mem_alloc(s)

#define FREE(p)                 mem_free(p)


void* mem_alloc(uint32_t size);
void  mem_free(void **ptr);

#endif