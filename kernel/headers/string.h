#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* dest, int val, size_t n);

#endif /* STRING_H */