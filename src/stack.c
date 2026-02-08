#include "uart.h"
#include "stack.h"

void check_stack_overflow(tcb_t *t)
{
    unsigned int guard =
        *(unsigned int *)(t->stack_base);

    if (guard != STACK_GUARD) {
        uart_puts("STACK OVERFLOW in task ");
        uart_put_int(t->id);
        uart_puts("\n");
        while (1); // kernel panic
    }
}

void update_stack_high_water(tcb_t *t)
{
    unsigned int used = 0;
    unsigned char *p = t->stack_base;

    // Skip guard word
    p += sizeof(unsigned int);

    for (unsigned int i = sizeof(unsigned int);
         i < t->stack_size;
         i++) {
        if (p[i - sizeof(unsigned int)] != STACK_PATTERN) {
            used = t->stack_size - i;
            break;
        }
    }

    if (used > t->stack_high_water) {
        t->stack_high_water = used;
    }
}


