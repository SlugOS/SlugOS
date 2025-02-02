// This is when something in the kernel crashes and its critcal

#include <slug.h>
#include <stdbool.h>
#include <drivers/vga.h>
#include <drivers/serial.h>
#include "errors.h"

void serial_log(int errorcode);
void pic_disable(void);
void IRQ_clear_mask(uint8_t IRQline);

bool debug = false;

// Function to return the error message for a specific error code
const char* get_error_message(int error_number) {
    // Return the corresponding error message from the array
    if (error_number < sizeof(errorcode) / sizeof(errorcode[0])) {
        return errorcode[error_number];
    }
    return "UNKNOWN ERROR";  // Fallback in case the error_number is out of bounds
}

void crash_banner() {
    printk("--------------------------------------------------------------------------------");
    printk("|                             SLUGOS HALTED SYSTEM                             |");
    printk("|                             Oops, something broke!                           |");
    printk("--------------------------------------------------------------------------------");
}

// This function is for a critical crash such as a CPU exception, a generic kernel panic, or SSP overflow and more
__attribute__((noreturn)) // This function does not return
void crash(unsigned int errorcode) {
    // First log to serial before we potentially mess with interrupts
    serial_log(errorcode);

    // Mask the PIC IRQs then enable the keyboard
    pic_disable();
    IRQ_clear_mask(1);
    
    // Set the text to be light grey and the background to be red
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_RED));
    clear_screen();
    crash_banner();
    
    // Print information about the error
    printk("ERROR: %s\n", get_error_message(errorcode));
    printk("ERROR CODE: %d\n", errorcode);
    
    // Run a stack trace if in debug mode
    #ifdef DEBUG
    debug = true;
    TraceStackTrace(10);
    printk("Debug Shell Type HELP for help\n");
    printk("> ");
    #else
    printk("Please reboot the system.\n");
    #endif
    
    // Halt the CPU
    while(1) {
        asm volatile("hlt");
    }
}

void serial_log(int errorcode) {
    // Clear the serial terminal
    printf_serial("\033[2J\033[H");
    // Report all of this info to the serial port + some extra stuff
    printf_serial("==== CRASH INFO ====\n");
    // Use snprintf to combine the error message and a newline into the buffer.
    printf_serial("ERROR: %s\n", get_error_message(errorcode));
    printf_serial("ERRORCODE: %d\n", errorcode);
    printf_serial("==== END OF CRASH INFO ====\n");
}