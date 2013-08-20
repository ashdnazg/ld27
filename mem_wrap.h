#ifndef __MEM_WRAP_H__
#define __MEM_WRAP_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdlib.h>


unsigned int g_mem_mem_allocs_count;


void mem_wrap_init();
void mem_wrap_print_mallocs();

void * mem_alloc(size_t size);
void * wrap_calloc(size_t size);
void mem_free(void *ptr);


#ifdef __cplusplus
}
#endif

#endif
