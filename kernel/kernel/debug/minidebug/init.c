/*
This code contains the init for the serial port and other required functions for minidebug
*/

#include <stddef.h>
#include <drivers/io.h>
#include "commands/common.h"

// Function prototypes
void dump_registers_serial();
void Serial_TraceStackTrace(unsigned int MaxFrames);
void inspect_memory(const char* addr_str);

#define PORT 0x3f8          // COM1

static int init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(PORT + 0) != 0xAE) {
      return 1;
   }

   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(PORT + 4, 0x0F);
   return 0;
}

static int serial_received() {
   return inb(PORT + 5) & 1;
}

static char read_serial() {
   while (serial_received() == 0);

   return inb(PORT);
}

static int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}

void putchar_serial(char a) {
   while (is_transmit_empty() == 0);

   outb(PORT,a);
}

void puts_serial(const char* str) {
   while (*str) {
      putchar_serial(*str);
      str++;
   }
}

char getchar_serial() {
   while (serial_received() == 0);
   return inb(PORT);
}

void minidebug_init() {
    if (init_serial() == 1) {
        puts_serial("Serial port is faulty. Exiting...\n");
        while(1);
    }
    debug_shell();
}

void debug_shell() {
    char input[256];
    int index;

    while (1) {
        puts_serial("\nminidebug> ");
        index = 0;

        // Read a line of input
        while (1) {
            char c = getchar_serial();

            if (c == '\r' || c == '\n') { // End of line
                input[index] = '\0';
                puts_serial("\r\n");
                break;
            } else if (c == '\b' && index > 0) { // Handle backspace
                index--;
                puts_serial("\b \b");
            } else if (index < 255) { // Store input until max length
                input[index++] = c;
                putchar_serial(c);
            }
        }

        // Command handling
        if (strcmp(input, "exit") == 0) {
            break;
        } else if (strcmp(input, "regs") == 0) {
            dump_registers_serial();
        } else if (strcmp(input, "trace") == 0) {
            Serial_TraceStackTrace(10);
        } else if (strcmp(input, "memory") == 0) {
            char addr_input[256];
            uint32_t addr;

            puts_serial("Enter memory address: ");
            index = 0;

            // Read memory address
            while (1) {
                char c = getchar_serial();
                if (c == '\r' || c == '\n') {
                    addr_input[index] = '\0';
                    puts_serial("\r\n");
                    break;
                } else if (c == '\b' && index > 0) {
                    index--;
                    puts_serial("\b \b");
                } else if (index < 255) {
                    addr_input[index++] = c;
                    putchar_serial(c);
                }
            }

            // Convert the entered address to hexadecimal
            addr = str_to_hex(addr_input);

            if (addr == 0xFFFFFFFF) {
                puts_serial("Invalid memory address\n");
            } else {
                inspect_memory(addr_input);
            }
        } else {
            puts_serial("Unknown command\n");
        }

        puts_serial("\r\n");
    }

    puts_serial("Exiting...\n");
}
