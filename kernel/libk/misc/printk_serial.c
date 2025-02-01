#include <stdarg.h>
#include <drivers/serial.h>

// Convert an integer to a string (base 10)
void itoa(int value, char *str, int base);

// This function simulates a printf but sends output via serial port.
int printf_serial(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    int index = 0;
    char c;
    while ((c = format[index++]) != '\0') {
        if (c == '%') {
            c = format[index++];  // Get the next character after '%'
            if (c == 'd') {
                // Handle integer printing
                int num = va_arg(args, int);
                char numStr[32];  // Temporary buffer for the number
                itoa(num, numStr, 10);  // Convert number to string in base 10
                for (int i = 0; numStr[i] != '\0'; i++) {
                    putchar_serial(numStr[i]);
                }
            } else if (c == 's') {
                // Handle string printing
                const char *str = va_arg(args, const char *);
                while (*str) {
                    putchar_serial(*str++);
                }
            } else {
                // Unknown specifier, print the character as-is
                putchar_serial('%');
                putchar_serial(c);
            }
        } else {
            // Regular character, just print it
            putchar_serial(c);
        }
    }
    
    va_end(args);
    return 0;
}
