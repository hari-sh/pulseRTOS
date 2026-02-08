#ifndef MEM_POOL_H
#define MEM_POOL_H

typedef struct mem_block {
    struct mem_block *next;
} mem_block_t;

typedef struct {
    unsigned int block_size;
    unsigned int total_blocks;

    unsigned int free_blocks;
    unsigned int min_free_blocks;   // lowest ever observed

    mem_block_t *free_list;
    unsigned char *pool_start;
} mem_pool_t;


void mem_pool_init(mem_pool_t *pool,
                   unsigned char *memory,
                   unsigned int block_size,
                   unsigned int block_count);

void *mem_alloc(mem_pool_t *pool);
void mem_free(mem_pool_t *pool, void *ptr);

#endif
