#ifndef UART_H
#define UART_H

void uart_putc(char c);
void uart_puts(const char *s);
void uart_put_uint(unsigned int x);
void uart_puts_2_uint(const char *fmt, unsigned int v1, unsigned int v2);
void uart_puts_2_int(const char *fmt, int v1, int v2);
void uart_put_int(int x);

#endif
