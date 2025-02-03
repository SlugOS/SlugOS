#include <slug.h>
#include <stdbool.h>
#include <drivers/vga.h>
#include <drivers/serial.h>
#include "errors.h"

// Function declarations
void serial_log(int errorcode);
void pic_disable(void);
void IRQ_clear_mask(uint8_t IRQline);

// This is what sets up the debug shell
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

// Simple crash handler
__attribute__((noreturn)) // This function does not return
void crash(unsigned int errorcode) {
    // First log to serial before we mess with interrupts
    serial_log(errorcode);

    // Mask the PIC IRQs then enable the keyboard
    pic_disable();
    IRQ_clear_mask(1); // Keyboard
    
    // Set the text to be light grey and the background to be red
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_RED));
    clear_screen();
    crash_banner();
    
    // Print information about the error
    printk("Error information:\n");
    printk("  ERROR: %s\n", get_error_message(errorcode));
    printk("  ERROR CODE: %d\n", errorcode);
    // Add the same info to the log
    add_to_log("Error information:");
    add_to_log("  ERROR: %s", get_error_message(errorcode));
    add_to_log("  ERROR CODE: %d", errorcode);
    
    // Start the debug shell (commands handled by keyboard driver)
    #ifdef DEBUG
    // Set debug to be true
    debug = true;

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
