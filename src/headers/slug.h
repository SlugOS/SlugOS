#pragma once

/*
The equivalent to dos.h in our case it contains the basics that are needed
*/

#include <stdint.h>

// Return status stuff

typedef int slug_status; // similar to EFI_STATUS

/*
SLUG_SUCCESS: When everything is fine and no failures occure this should be returned
SLUG_ERROR: This is a generic error message for when the error is unknown or something else happens
SLUG_UNSUPPORTED: When something like hardware or some other thing is not supported or is just a placeholder
*/
#define SLUG_SUCCESS 0
#define SLUG_ERROR 1
#define SLUG_UNSUPPORTED 2

// Terminal related functions

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// Write a single character to the VGA text mode used for user output
void putchar(char c);
// Write a string to the VGA text mode used for user output
void writestring(const char* data);
// Set the VGA text mode text color with the vga_color enum used for logging
void terminal_setcolor(uint8_t color);

// Print to the serial port
void print_serial(char* data);

// Formatted print
__attribute__ ((format (printf, 1, 2))) int printf (const char* format, ...);

// 8bit data being sent to a port
void outb(uint16_t port, uint8_t val);
// 8bit data being recivied from a port
uint8_t inb(uint16_t port);

// 16bit data being sent to a port
void outw(uint16_t port, uint16_t val);

// 32bit input from port
uint32_t inl(uint16_t port);
// 32bit output to a port
void outl(uint16_t port, uint32_t value);

// 16bit data being recivied from a port
uint16_t inw(uint16_t port);

// Impercise wait about 1 to 4 nanoseconds
void io_wait(void);

// Stack Frame
typedef struct {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp; // Stored manually
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t interrupt_number;
    uint32_t error_code; // Some interrupts don't push an error code
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t user_esp; // Optional, only for privilege transitions
    uint32_t ss;       // Optional, only for privilege transitions
} __attribute__((packed)) interrupt_stack_frame_t;

/*
klog is a simple logger these are the levels

level 0: DEBUG info that goes to the serial port
level 1: INFO which is just info about what is happening and what was initalized
level 2: WARN which is for warnings about something not working
level 3: ERR which is when something is not working and it will not be able to work
level 4: CRIT ERR which halts the CPU and disables interrupts
*/
void klog(int level, char* data);