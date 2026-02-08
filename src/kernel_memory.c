#include "mem_pool.h"
#include "tcb.h"
#include "uart.h"

#define POOL16_COUNT   32
#define POOL32_COUNT   32
#define POOL64_COUNT   16
#define POOL128_COUNT  8
#define TCB_COUNT  8


static unsigned char pool16_mem[16 * POOL16_COUNT];
static unsigned char pool32_mem[32 * POOL32_COUNT];
static unsigned char pool64_mem[64 * POOL64_COUNT];
static unsigned char pool128_mem[128 * POOL128_COUNT];
static unsigned char tcb_pool_mem[sizeof(tcb_t) * TCB_COUNT];


mem_pool_t pool16;
mem_pool_t pool32;
mem_pool_t pool64;
mem_pool_t pool128;
mem_pool_t tcb_pool;


void kernel_mem_init(void)
{
    mem_pool_init(&pool16, pool16_mem, 16, POOL16_COUNT);
    mem_pool_init(&pool32, pool32_mem, 32, POOL32_COUNT);
    mem_pool_init(&pool64, pool64_mem, 64, POOL64_COUNT);
    mem_pool_init(&pool128, pool128_mem, 128, POOL128_COUNT);
    mem_pool_init(&tcb_pool, tcb_pool_mem, sizeof(tcb_t), TCB_COUNT);
}


void kernel_mem_dump(void)
{
    uart_puts("Kernel memory stats:\n");

    uart_puts("Pool16  free=");
    uart_put_uint(pool16.free_blocks);
    uart_puts(" min=");
    uart_put_uint(pool16.min_free_blocks);
    uart_puts("\n");

    uart_puts("Pool32  free=");
    uart_put_uint(pool32.free_blocks);
    uart_puts(" min=");
    uart_put_uint(pool32.min_free_blocks);
    uart_puts("\n");

    uart_puts("Pool64  free=");
    uart_put_uint(pool64.free_blocks);
    uart_puts(" min=");
    uart_put_uint(pool64.min_free_blocks);
    uart_puts("\n");

    uart_puts("Pool128 free=");
    uart_put_uint(pool128.free_blocks);
    uart_puts(" min=");
    uart_put_uint(pool128.min_free_blocks);
    uart_puts("\n");
}

