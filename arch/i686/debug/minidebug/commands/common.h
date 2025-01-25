#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

void int_to_hex(uint32_t value, char* buffer);
void puts_serial(const char* str);
uint32_t str_to_hex(const char* str);

#endif