#include <stdint.h>

// Output a byte (8 bits)
void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

// Input a byte (8 bits)
uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0"
                     : "=a"(ret)
                     : "Nd"(port)
                     : "memory");
    return ret;
}

// Output a word (16 bits)
void outw(uint16_t port, uint16_t val) {
    __asm__ volatile("outw %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

// Input a word (16 bits)
uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile("inw %w1, %w0"
                     : "=a"(ret)
                     : "Nd"(port)
                     : "memory");
    return ret;
}

// Output a double word (32 bits)
void outl(uint16_t port, uint32_t val) {
    __asm__ volatile("outl %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

// Input a double word (32 bits)
uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile("inl %w1, %0"
                     : "=a"(ret)
                     : "Nd"(port)
                     : "memory");
    return ret;
}

// Function to wait about 1 to 4 nanoseconds
void io_wait(void) {
    outb(0x80, 0);
}
