#ifndef HLD_UART_H
#define HLD_UART_H

#include <stdint.h>

void uart1_init(uint32_t baudrate);

void uart1_puts(const char* string);

void uart1_putc(char c);


#endif
