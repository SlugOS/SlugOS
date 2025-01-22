#include <slug.h>
#include <string.h>
#include <drivers/vga.h>
#include <boot/multiboot1.h>

extern "C" void install_gdt();
extern "C" void idt_init();

bool quiet;

extern "C" void kernel_main(multiboot_info_t *multiboot_info) {
    // Setup the GDT this code allows us to use interrupts
    install_gdt();
    // Setup the interrupts
    idt_init();
    // Setup VGA text mode so printk and other functions work
    terminal_initialize();
    // Check that quiet flag is not enabled
    if (strstr((const char*)multiboot_info->cmdline, "quiet") == NULL) {
	    quiet = true;
        printk("Thank you for using SlugOS\n");
    } else {
        // Quiet is enabled
        quiet = false;
    }
}
