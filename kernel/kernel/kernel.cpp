#include <slug.h>
#include <string.h>
#include <drivers/vga.h>
#include <boot/multiboot1.h>

extern "C" void kernel_main(multiboot_info_t *multiboot_info) {
    terminal_initialize();
	printk("Thank you for using SlugOS\n");
}
