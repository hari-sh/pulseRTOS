#include "uart.h"

#define UART0_BASE 0x10000000UL
#define UART_THR (*(volatile unsigned char *)(UART0_BASE + 0x00))
#define UART_LSR (*(volatile unsigned char *)(UART0_BASE + 0x05))
#define LSR_TX_IDLE 0x20

void uart_putc(char c)
{
    while ((UART_LSR & LSR_TX_IDLE) == 0);
    UART_THR = c;
}

void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}

void uart_put_uint(unsigned int x)
{
    static const unsigned int pow10[] = {
        1000000000U,
        100000000U,
        10000000U,
        1000000U,
        100000U,
        10000U,
        1000U,
        100U,
        10U,
        1U
    };

    int started = 0;

    for (int i = 0; i < 10; i++) {
        unsigned int digit = 0;

        while (x >= pow10[i]) {
            x -= pow10[i];
            digit++;
        }

        if (digit || started || i == 9) {
            uart_putc('0' + digit);
            started = 1;
        }
    }
}


void uart_puts_2_uint(const char *fmt, unsigned int v1, unsigned int v2)
{
    int used = 0;

    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1) == 'd') {
            if (used == 0)
                uart_put_uint(v1);
            else if (used == 1)
                uart_put_uint(v2);

            used++;
            fmt += 2;
            continue;
        }

        uart_putc(*fmt++);
    }
}


void uart_put_int(int x)
{
    if (x < 0) {
        uart_putc('-');

        /* careful: avoid overflow for INT_MIN */
        unsigned int ux = (unsigned int)(-(x + 1)) + 1;
        uart_put_uint(ux);
    } else {
        uart_put_uint((unsigned int)x);
    }
}


void uart_puts_2_int(const char *fmt, int v1, int v2)
{
    int used = 0;

    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1) == 'd') {
            if (used == 0)
                uart_put_int(v1);
            else if (used == 1)
                uart_put_int(v2);

            used++;
            fmt += 2;
            continue;
        }

        uart_putc(*fmt++);
    }
}
