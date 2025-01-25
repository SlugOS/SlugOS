#include <slug.h>
#include <string.h>
#include <stdbool.h>
#include <drivers/vga.h>
#include <boot/multiboot1.h>

void initialize();
#ifdef i686
void jump_usermode();
#endif

bool quiet;

// This is a simple entry point to the kernel.
void kernel_main(multiboot_info_t *multiboot_info) {
    initialize();
    // Check the quiet flag
    if (strstr(multiboot_info->cmdline, "quiet") == NULL) {
        quiet = false;
        displaylogo();
    } else {
        // Quiet is enabled
        quiet = true;
    }
    #ifdef RSOD
    // Force a SSP call to test the crash code
    char data[10];
    strcpy(data, "123456789\n");
    #endif

    #ifdef i686
    jump_usermode();
    #endif
}