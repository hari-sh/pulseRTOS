#include "mem_pool.h"

void mem_pool_init(mem_pool_t *pool,
                   unsigned char *memory,
                   unsigned int block_size,
                   unsigned int block_count)
{
    pool->block_size = block_size;
    pool->total_blocks = block_count;
    pool->free_blocks = block_count;
    pool->min_free_blocks = block_count;
    pool->pool_start = memory;
    pool->free_list = 0;

    for (unsigned int i = 0; i < block_count; i++) {
        mem_block_t *blk =
            (mem_block_t *)(memory + i * block_size);
        blk->next = pool->free_list;
        pool->free_list = blk;
    }
}


void *mem_alloc(mem_pool_t *pool)
{
    if (!pool->free_list)
        return 0;

    mem_block_t *blk = pool->free_list;
    pool->free_list = blk->next;

    pool->free_blocks--;
    if (pool->free_blocks < pool->min_free_blocks)
        pool->min_free_blocks = pool->free_blocks;

    return (void *)blk;
}


void mem_free(mem_pool_t *pool, void *ptr)
{
    mem_block_t *blk = (mem_block_t *)ptr;
    blk->next = pool->free_list;
    pool->free_list = blk;

    pool->free_blocks++;
}

