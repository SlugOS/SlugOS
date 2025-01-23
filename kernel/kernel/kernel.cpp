#include <slug.h>
#include <string.h>
#include <drivers/vga.h>
#include <boot/multiboot1.h>

void initalize();

bool quiet;

extern "C" void kernel_main(multiboot_info_t *multiboot_info) {
    initalize();
    // Check that quiet flag is not enabled
    if (strstr((const char*)multiboot_info->cmdline, "quiet") == NULL) {
	    quiet = false;
        displaylogo();
    } else {
        // Quiet is enabled
        quiet = true;
    }
    #ifdef RSOD
    // Force a SSP overflow
    char data[10];
    strcpy(data, "123456789\n");
    #endif
}
