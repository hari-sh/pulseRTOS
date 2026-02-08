#include "mem_pool.h"

#define POOL16_COUNT   32
#define POOL32_COUNT   32
#define POOL64_COUNT   16
#define POOL128_COUNT  8

static unsigned char pool16_mem[16 * POOL16_COUNT];
static unsigned char pool32_mem[32 * POOL32_COUNT];
static unsigned char pool64_mem[64 * POOL64_COUNT];
static unsigned char pool128_mem[128 * POOL128_COUNT];

mem_pool_t pool16;
mem_pool_t pool32;
mem_pool_t pool64;
mem_pool_t pool128;

void kernel_mem_init(void)
{
    mem_pool_init(&pool16, pool16_mem, 16, POOL16_COUNT);
    mem_pool_init(&pool32, pool32_mem, 32, POOL32_COUNT);
    mem_pool_init(&pool64, pool64_mem, 64, POOL64_COUNT);
    mem_pool_init(&pool128, pool128_mem, 128, POOL128_COUNT);
}
