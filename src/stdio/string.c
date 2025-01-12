#include <stddef.h>

// Copies data between two locations with a specified length
void movedata(void *src, void *dest, size_t len) {
    unsigned char *source = (unsigned char *)src;
    unsigned char *destination = (unsigned char *)dest;

    // If the source and destination are the same, no need to copy
    if (source == destination) {
        return;
    }

    // Copy data byte by byte
    for (size_t i = 0; i < len; i++) {
        destination[i] = source[i];
    }
}

// Same as movedata, designed specifically for copying memory blocks
void *memcpy(void *dest, const void *src, size_t len) {
    unsigned char *destination = (unsigned char *)dest;
    const unsigned char *source = (const unsigned char *)src;

    // Copy data byte by byte
    for (size_t i = 0; i < len; i++) {
        destination[i] = source[i];
    }

    return dest;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}