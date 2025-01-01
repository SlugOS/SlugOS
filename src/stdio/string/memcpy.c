#include <stddef.h>

void* memcpy(void* dest, const void* src, size_t num) {
    unsigned char* d = dest;
    const unsigned char* s = src;
    while (num--) {
        *d++ = *s++;
    }
    return dest;
}
