// This is when something in the kernel crashes and its critcal

#include <slug.h>
#include <drivers/vga.h>
#include "code.h"

void TraceStackTrace(unsigned int MaxFrames);

// Function to return the error message for a specific error code
const char* get_error_message(int error_number) {
    // Return the corresponding error message from the array
    if (error_number < sizeof(errorcode) / sizeof(errorcode[0])) {
        return errorcode[error_number];
    }
    return "UNKNOWN ERROR";  // Fallback in case the error_number is out of bounds
}

// This function is called to crash the OS with an error code that can be a negitive and a positive
__attribute__((noreturn)) // This function does not return
void crash(unsigned int errorcode) {
    asm("cli"); // Disable interrupts so nothing can get us out of this screen
    // Set the text to be light grey and the background to be red
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_RED));
    // Clear the screen
    clear_screen();
    // Print our logo
    displaylogo();
    // Print out our banner
    printk("--------------------------------------------------------------------------------");
    printk("|                             SLUGOS HALTED SYSTEM                             |");
    printk("|                             Oops, something broke!                           |");
    printk("--------------------------------------------------------------------------------");
    // Print the errorcode
    printk("ERROR CODE: %d\n", errorcode);
    // Print more specific info about the error
    printk("ERROR TYPE: %s\n", get_error_message(errorcode));
    TraceStackTrace(10);
    asm("hlt");
}