#include <stdarg.h>
#include <vga.h>

// Converts an integer to a string (decimal, signed)
static void int_to_str(int value, char* buffer) {
    char temp[20];
    int i = 0, j = 0;
    
    if (value == 0) {
        temp[i++] = '0';
    } else {
        if (value < 0) {  // Handle negative integers
            temp[i++] = '-';
            value = -value;
        }
        while (value > 0) {
            temp[i++] = (value % 10) + '0';  // Convert to digit character
            value /= 10;
        }
    }
    
    // Reverse the string (ignoring the '-' sign if it's negative)
    for (j = (temp[0] == '-') ? 1 : 0; i > j; i--, j++) {
        buffer[j] = temp[i - 1];
    }
    buffer[j] = '\0';
}

// Converts an integer to a hexadecimal string (lowercase)
static void int_to_hex(int value, char* buffer) {
    const char hex_digits[] = "0123456789abcdef";
    char temp[9];  // 8 digits + null terminator for 32-bit numbers
    int i = 0, j = 0;
    
    if (value == 0) {
        temp[i++] = '0';
    } else {
        while (value > 0) {
            temp[i++] = hex_digits[value % 16];  // Convert to hex character
            value /= 16;
        }
    }
    
    // Reverse the string
    for (j = 0; i > j; i--, j++) {
        buffer[j] = temp[i - 1];
    }
    buffer[j] = '\0';
}

// A simple implementation of printf
void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;  // Move past '%'

            if (*fmt == 'c') { // Handle %c (char)
                char c = va_arg(args, char);
                terminal_putchar(c);
            }
            else if (*fmt == 's') { // Handle %s (string)
                const char* str = va_arg(args, const char*);
                print(str);
            }
            else if (*fmt == 'd') { // Handle %d (decimal integer)
                int num = va_arg(args, int);
                char buffer[20];  // Buffer for integer string
                int_to_str(num, buffer);
                print(buffer);
            }
            else if (*fmt == 'x') { // Handle %x (hexadecimal integer)
                int num = va_arg(args, int);
                char buffer[9];  // Buffer for hexadecimal string
                int_to_hex(num, buffer);
                print(buffer);
            }
        }
        else { // Regular character
            terminal_putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}
