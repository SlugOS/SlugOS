#include <slug.h>
#include <string.h>
#include <stdbool.h>
#include <drivers/vga.h>
#include <boot/multiboot1.h>

void initialize();
#ifdef i686
void jump_usermode();
#endif

// This is a simple entry point to the kernel.
void kernel_main() {
    initialize();
    displaylogo();
    #ifdef RSOD
    // Force a SSP call to test the crash code
    char data[10];
    strcpy(data, "123456789\n");
    #endif

    #ifdef i686
    jump_usermode();
    #endif
}