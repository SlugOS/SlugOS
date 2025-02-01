#include <stdarg.h>

// Convert an integer to a string (base 10)
void itoa(int value, char *str, int base) {
    int index = 0;
    int isNegative = 0;

    // Handle 0 explicitly, otherwise empty string is printed
    if (value == 0) {
        str[index++] = '0';
        str[index] = '\0';
        return;
    }

    // Handle negative numbers (only for base 10 here)
    if (value < 0 && base == 10) {
        isNegative = 1;
        value = -value;
    }

    // Process individual digits
    while (value != 0) {
        int rem = value % base;
        str[index++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';  // Get the character for the digit
        value = value / base;
    }

    // Append negative sign for negative numbers
    if (isNegative) {
        str[index++] = '-';
    }

    str[index] = '\0';  // Null-terminate the string

    // Reverse the string, because we've been adding digits from least significant to most
    int start = 0;
    int end = index - 1;
    while (start < end) {
        // Swap characters at start and end
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int snprintf(char *buffer, int size, const char *format, ...) {
    int index = 0;  // Current write index in the buffer
    va_list args;
    va_start(args, format);

    // Loop over each character in the format string.
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            // Write the character directly if there's space.
            if (index < size - 1) {
                buffer[index++] = format[i];
            } else {
                // Buffer full: exit the loop to ensure safe termination.
                break;
            }
        } else {
            // We encountered a '%' character.
            i++;  // Move to the format specifier character.
            if (format[i] == 's') {
                // Handle string: retrieve next argument.
                const char *str = va_arg(args, const char *);
                // Append the string character by character.
                for (int j = 0; str[j] != '\0'; j++) {
                    if (index < size - 1) {
                        buffer[index++] = str[j];
                    } else {
                        break;  // No more space.
                    }
                }
            } else if (format[i] == 'd') {
                // Handle integer: retrieve next argument.
                int num = va_arg(args, int);
                // Convert the integer to a string.
                // Here, we assume a helper function itoa exists.
                char numStr[32]; // Temporary buffer for the number.
                itoa(num, numStr, 10);  // Convert num to a string in base 10.
                // Append the number string.
                for (int j = 0; numStr[j] != '\0'; j++) {
                    if (index < size - 1) {
                        buffer[index++] = numStr[j];
                    } else {
                        break;
                    }
                }
            } else {
                // For unknown specifiers, simply write the character as-is.
                if (index < size - 1) {
                    buffer[index++] = format[i];
                }
            }
        }
    }
    
    // Ensure the buffer is null-terminated.
    if (size > 0) {
        buffer[index < size ? index : size - 1] = '\0';
    }

    va_end(args);
    return index;  // Return the number of characters written.
}