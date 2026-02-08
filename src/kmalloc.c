#include "mem_pool.h"

extern mem_pool_t pool16, pool32, pool64, pool128;

void *kmalloc(unsigned int size)
{
    if (size <= 16)  return mem_alloc(&pool16);
    if (size <= 32)  return mem_alloc(&pool32);
    if (size <= 64)  return mem_alloc(&pool64);
    if (size <= 128) return mem_alloc(&pool128);
    return 0;
}

void kfree(void *ptr, unsigned int size)
{
    if (size <= 16)  mem_free(&pool16, ptr);
    else if (size <= 32) mem_free(&pool32, ptr);
    else if (size <= 64) mem_free(&pool64, ptr);
    else if (size <= 128) mem_free(&pool128, ptr);
}
