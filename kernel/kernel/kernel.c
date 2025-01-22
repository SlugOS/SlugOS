#include <slug.h>

#ifdef DEBUG
extern void test_ne2k();
#endif

void kernel_main(void) {
	printk("Thank you for using SlugOS\n");
	#ifdef DEBUG
	test_ne2k();
	#endif
}
