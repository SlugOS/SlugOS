/*
Show the value of the memory in the address
*/

#include "common.h"

void inspect_memory(const char* addr_str) {
    char hex_buffer[12]; // Buffer for hex conversion
    uint32_t addr = str_to_hex(addr_str);

    if (addr == 0xFFFFFFFF) {
        puts_serial("Invalid address format\n");
        return;
    }

    uint32_t value = *(volatile uint32_t*)addr; // Dereference the address

    puts_serial("Memory at ");
    int_to_hex(addr, hex_buffer);
    puts_serial(hex_buffer);

    puts_serial(": ");
    int_to_hex(value, hex_buffer);
    puts_serial(hex_buffer);
    puts_serial("\n");
}
