// This is when something in the kernel crashes and its critcal

#include <slug.h>
#include <drivers/vga.h>

// A simple serial shell for debugging
void debug_shell();

const char* errorcode[] = {
    "DIVISION ERROR",            // 0
    "DEBUG",                     // 1
    "NON-MASKABLE INTERRUPT",    // 2
    "BREAKPOINT",                // 3
    "OVERFLOW",                  // 4
    "BOUND RANGE EXCEEDED",      // 5
    "INVALID OPCODE",            // 6
    "DEVICE NOT AVAILABLE",      // 7
    "DOUBLE FAULT",              // 8
    "COPROCESSOR SEGMENT OVERRUN", // 9
    "INVALID TSS",               // 10
    "SEGMENT NOT PRESENT",       // 11
    "STACK-SEGMENT FAULT",       // 12
    "GENERAL PROTECTION FAULT",  // 13
    "PAGE FAULT",                // 14
    "RESERVED",                  // 15
    "x87 FLOATING-POINT EXCEPTION", // 16
    "ALIGNMENT CHECK",           // 17
    "MACHINE CHECK",             // 18
    "SIMD FLOATING-POINT EXCEPTION", // 19
    "VIRTUALIZATION EXCEPTION",  // 20
    "CONTROL PROTECTION EXCEPTION", // 21
    "RESERVED",                  // 22
    "RESERVED",                  // 23
    "RESERVED",                  // 24
    "RESERVED",                  // 25
    "HYPERVISOR INJECTION EXCEPTION", // 26
    "VMM COMMUNICATION EXCEPTION", // 27
    "SECURITY EXCEPTION",        // 28
    "RESERVED",                  // 29
    "FPU ERROR INTERRUPT",       // 30
    "SSP VIOLATION",             // 31
    "KERNEL PANIC (GENERIC)",    // 32
};

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
    #ifdef DEBUG
    // Start the debug shell
    debug_shell();
    #endif
    while(1); // Loop forever to keep the system halted since interrupts are disabled
}