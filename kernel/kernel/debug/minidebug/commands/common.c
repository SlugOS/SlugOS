/*
This is a bunch of common functions that are used with the rest of the commands
*/

#include <stdint.h>

void int_to_hex(uint32_t value, char* buffer) {
    const char hex_chars[] = "0123456789ABCDEF";
    buffer[0] = '0';
    buffer[1] = 'x';

    for (int i = 0; i < 8; i++) {
        buffer[9 - i] = hex_chars[value & 0xF]; // Extract the last nibble
        value >>= 4;                            // Shift right by 4 bits
    }

    buffer[10] = '\0'; // Null-terminate the string
}

uint32_t str_to_hex(const char* str) {
    uint32_t result = 0;

    // Handle optional "0x" prefix
    if (str[0] == '0' && str[1] == 'x') {
        str += 2; // Skip "0x"
    }

    // Convert each character
    while (*str) {
        char c = *str++;
        result <<= 4; // Shift left 4 bits to make space for the next nibble
        if (c >= '0' && c <= '9') {
            result |= (c - '0');
        } else if (c >= 'A' && c <= 'F') {
            result |= (c - 'A' + 10);
        } else if (c >= 'a' && c <= 'f') {
            result |= (c - 'a' + 10);
        } else {
            return 0xFFFFFFFF; // Invalid character, return error value
        }
    }

    return result;
}
