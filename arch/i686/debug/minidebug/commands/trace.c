/*
Do a simple stack trace and output to the serial port
*/

#include "common.h"
#include <stdint.h>
#include <slug.h>

// Struct representing a stack frame
struct stackframe {
    struct stackframe* ebp;
    uint32_t eip;
};

// Trace the stack up to `MaxFrames` levels
void Serial_TraceStackTrace(unsigned int MaxFrames) {
    struct stackframe* stk;
    char hex_buffer[12]; // Enough to store "0x12345678\0"

    // Get the current base pointer
    asm ("movl %%ebp, %0" : "=r"(stk) ::);

    // Print the stack trace header
    puts_serial("Stack trace:\n");

    // Traverse the stack
    for (unsigned int frame = 0; stk && frame < MaxFrames; ++frame) {
        // Convert EIP to hex
        int_to_hex(stk->eip, hex_buffer);
        
        // Print the formatted EIP address
        puts_serial("  ");
        puts_serial(hex_buffer);
        puts_serial("\n");

        // Move to the next stack frame
        stk = stk->ebp;
    }
}
