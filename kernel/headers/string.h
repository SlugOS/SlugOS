#ifndef STRING_H
#define STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* dest, int val, size_t n);
char *strstr(const char *haystack, const char *needle);
int strcmp(const char *a, const char *b);

#ifdef __cplusplus
}
#endif

#endif /* STRING_H */