#pragma once

#include <stddef.h>

// Copies data between two locations with a specified length
void movedata(void *src, void *dest, size_t len);
// Same as movedata, designed specifically for copying memory blocks
void *memcpy(void *dest, const void *src, size_t len);
// Find the string length
size_t strlen(const char* str);
// Compare strings
int strcmp(const char *str1, const char *str2);