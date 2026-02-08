#include "mem_pool.h"
#include "panic.h"

extern mem_pool_t pool16, pool32, pool64, pool128;

void *kmalloc(unsigned int size)
{
    void *ptr = 0;

    if (size <= 16)       ptr = mem_alloc(&pool16);
    else if (size <= 32)  ptr = mem_alloc(&pool32);
    else if (size <= 64)  ptr = mem_alloc(&pool64);
    else if (size <= 128) ptr = mem_alloc(&pool128);
    else
        kernel_panic("kmalloc size too large");

    if (!ptr)
        kernel_panic("Out of kernel memory");

    return ptr;
}


void kfree(void *ptr, unsigned int size)
{
    if (size <= 16)  mem_free(&pool16, ptr);
    else if (size <= 32) mem_free(&pool32, ptr);
    else if (size <= 64) mem_free(&pool64, ptr);
    else if (size <= 128) mem_free(&pool128, ptr);
}
