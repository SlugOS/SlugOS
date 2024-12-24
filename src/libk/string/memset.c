#include <stddef.h>

void* memset(void* dest, int val, size_t size) {
    char* d = (char*)dest;
    for (size_t i = 0; i < size; i++) {
        d[i] = (char)val;
    }
    return dest;
}