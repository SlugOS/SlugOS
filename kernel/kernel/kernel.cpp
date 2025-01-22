#include <slug.h>

void _init();

extern "C" void kernel_main(void) {
	_init();
	printk("Thank you for using SlugOS\n");
	asm("hlt");
}
