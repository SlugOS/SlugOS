#ifndef SLUG_H
#define SLUG_H

// The standard way to print out a single character
void putchar(char c);
// Unformatted way to print a string
void writestring(const char* data);

// A simple version of printf but meant for the kernelspace
__attribute__ ((format (printk, 1, 2))) int printk (const char* format, ...);

#endif /* SLUG_H */