#include <slug.h>

void kernel_main(void) {
	printk("Thank you for using SlugOS\n");
	#ifdef DEBUG
	TraceStackTrace(10);
	#endif
}
