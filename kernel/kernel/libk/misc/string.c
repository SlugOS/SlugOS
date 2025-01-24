#include <stddef.h>

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

char* strcpy(char* dest, const char* src) {
    char* ptr = dest;
    while ((*ptr++ = *src++));  // Copy each character, including the null terminator
    return dest;
}

char* strcat(char* dest, const char* src) {
    char* ptr = dest;
    while (*ptr) ptr++;           // Find the end of dest
    while ((*ptr++ = *src++));    // Append src
    return dest;
}

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*) dest;
    const unsigned char* s = (const unsigned char*) src;
    while (n--) *d++ = *s++;
    return dest;
}

void* memset(void* dest, int val, size_t n) {
    unsigned char* d = (unsigned char*) dest;
    while (n--) *d++ = (unsigned char) val;
    return dest;
}

char *strstr(const char *haystack, const char *needle) {
    if (!*needle) // If needle is empty, return haystack
        return (char *)haystack;

    for (; *haystack; haystack++) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && *h == *n) {
            h++;
            n++;
        }

        if (!*n) // If we reached the end of needle, return start of the match
            return (char *)haystack;
    }

    return NULL; // No match found
}

int strcmp(const char *a, const char *b) {
    while (*a && *a == *b) { ++a; ++b; }
    return (int)(unsigned char)(*a) - (int)(unsigned char)(*b);
}