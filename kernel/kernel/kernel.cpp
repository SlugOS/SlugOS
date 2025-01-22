#include <slug.h>
#include <drivers/vga.h>

extern "C" void kernel_main(void) {
    terminal_initialize();
	printk("Thank you for using SlugOS\n");
}
