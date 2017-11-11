#ifndef SERIAL_STDIO_H
#define SERIAL_STDIO_H

void serial_printf(void (*p_puts)(const char *), const char * format, ...)
    __attribute__ ((format (printf, 2, 3)));

#endif// SERIAL_STDIO_H
