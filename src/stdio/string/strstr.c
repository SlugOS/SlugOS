#include <stdio.h>
#include <stddef.h>

char *strstr(const char *haystack, const char *needle) {
    if (*needle == '\0') {
        return (char *)haystack;  // If needle is empty, return the full haystack
    }

    for (const char *h = haystack; *h != '\0'; ++h) {
        const char *p = h;
        const char *q = needle;

        while (*p == *q && *q != '\0') {
            ++p;
            ++q;
        }

        if (*q == '\0') {
            return (char *)h;  // Found the needle
        }
    }
    
    return NULL;  // Needle not found
}
