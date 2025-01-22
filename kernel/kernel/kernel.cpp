#include <slug.h>

extern "C" void kernel_main(void) {
	printk("Thank you for using SlugOS\n");
	asm("hlt");
}
