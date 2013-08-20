#include "mem_wrap.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void * mem_alloc(size_t size) {
    void *ptr = malloc(size);
    assert(ptr != NULL);
    ++g_mem_mem_allocs_count;
    return ptr;
}

void mem_free(void *ptr) {
    --g_mem_mem_allocs_count;
    free(ptr);
}

void mem_wrap_init() {
    g_mem_mem_allocs_count = 0;
}

void mem_wrap_print_mallocs() {
    printf("\nUnfreed mallocs: %d", g_mem_mem_allocs_count);
}
