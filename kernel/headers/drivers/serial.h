#ifndef SERIAL_H
#define SERIAL_H

int init_serial();
void putchar_serial(char a);
void puts_serial(const char* str);
int printf_serial(const char *format, ...);

#endif