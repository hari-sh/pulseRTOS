#include "uart.h"

void kernel_panic(const char *msg)
{
    uart_puts("\nKERNEL PANIC: ");
    uart_puts(msg);
    uart_puts("\n");

    while (1);
}
