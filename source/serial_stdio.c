#include "serial_stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void serial_printf(void (*p_puts)(const char *), const char * format, ...){
    char * tempBuffer;
    int bufferSize;

    va_list args;
    va_start(args, format);

    bufferSize = vsnprintf(NULL, 0,format, args);
    bufferSize++;
    tempBuffer = (char*) malloc(bufferSize);
    vsnprintf(tempBuffer,bufferSize,format,args);

    va_end(args);

    p_puts(tempBuffer);
    free(tempBuffer);
}
