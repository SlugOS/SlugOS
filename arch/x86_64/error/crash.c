// This is when something in the kernel crashes and its critcal

#include <slug.h>
#include <drivers/vga.h>
#include <drivers/serial.h>
#include "errors.h"

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

void serial_log(int errorcode);

// This function is for a critical crash such as a CPU exception, a generic kernel panic, or SSP overflow and more
__attribute__((noreturn)) // This function does not return
void crash(unsigned int errorcode) {
    // Disable interrupts so we cannot exit the RSOD
    asm("cli");
    // Set the text to be light grey and the background to be red
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_RED));
    clear_screen();
    crash_banner();
    // Print information about the error
    printk("ERROR: %s\n", get_error_message(errorcode));
    printk("ERROR CODE: %d\n", errorcode);
    #ifdef DEBUG
    // Run a stack trace
    TraceStackTrace(10);
    #endif
    printk("Please reboot the system.\n");
    // Log to serial
    serial_log(errorcode);
    asm("hlt");
}

void serial_log(int errorcode) {
    // Report all of this info to the serial port + some extra stuff
    puts_serial("==== CRASH INFO ====\n");
    // Use snprintf to combine the error message and a newline into the buffer.
    printf_serial("ERROR: %s\n", get_error_message(errorcode));
    printf_serial("ERRORCODE: %d\n", errorcode);
    puts_serial("==== END OF CRASH INFO ====\n");
}