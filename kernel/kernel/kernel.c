#include <slug.h>
#include <drivers/vga.h>

#ifdef RSOD
#include <string.h>
#endif

void initialize();
int thread_test();

// This is a simple entry point to the kernel.
void kernel_main() {
    initialize();
    add_to_log("Kernel initialized");
    displaylogo();
    #ifdef RSOD
    // Force a SSP call to test the crash code
    char data[10];
    strcpy(data, "123456789\n");
    #endif

    #ifdef DEBUG
    thread_test();
    add_to_log("Thread test complete");
    #endif
}