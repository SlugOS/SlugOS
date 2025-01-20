#include <slug.h>

void kernel_main(void) {
	printk("Thank you for using SlugOS\n");
	#ifdef DEBUG
	printk("Decimal test: %d\n", 1234);
	printk("Hex test: 0x%x\n", 0xFF);
	#endif
}
