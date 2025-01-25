/*
This is the actual shell of the minidebug shell.
*/

#include <stdint.h>
#include "commands/common.h"

// Function prototypes
void dump_registers_serial();
void Serial_TraceStackTrace(unsigned int MaxFrames);
void inspect_memory(const char* addr_str);

char* getstring_serial();

void debug_shell() {
    puts_serial("Please type 'help' for a list of commands.\n");
    char* input;
    int index;

    while (1) {
        puts_serial("\nminidebug> ");
        index = 0;

        input = getstring_serial();

        // Command handling
        if (strcmp(input, "help") == 0) {
            puts_serial("Available commands:\n");
            puts_serial("  help - Display this help message\n");
            puts_serial("  regs - Display the current CPU registers\n");
            puts_serial("  trace - Trace the execution of the program\n");
            puts_serial("  memory - Inspect a memory location\n");
            puts_serial("  exit - Exit the debug shell\n");
        } else if (strcmp(input, "regs") == 0) {
            dump_registers_serial();
        } else if (strcmp(input, "trace") == 0) {
            char* size_input;
            uint32_t size;

            puts_serial("Enter trace iterations: ");
            size_input = getstring_serial();

            // Convert the entered address to hexadecimal
            size = str_to_hex(size_input);
            Serial_TraceStackTrace(size);
        } else if (strcmp(input, "memory") == 0) {
            char* addr_input;
            uint32_t addr;

            puts_serial("Enter memory address: ");
            addr_input = getstring_serial();

            // Convert the entered address to hexadecimal
            addr = str_to_hex(addr_input);

            if (addr == 0xFFFFFFFF) {
                puts_serial("Invalid memory address\n");
            } else {
                inspect_memory(addr_input);
            }
        } else if (strcmp(input, "exit") == 0) {
            break;
        } else {
            puts_serial("Unknown command\n");
        }
    }

    puts_serial("Exiting...\n");
}
