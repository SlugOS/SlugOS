#include <slug.h>
#ifdef DEBUG
#include <liballoc.h>
#include <string.h>
#endif

#ifdef DEBUG
int liballoc_test() {
	void* data = malloc(5);
	if (data == NULL) {
		return 1;
	}
	strcpy(data, "hi!\n");
	printk("Liballoc test: %s", data);
	free(data);
	return 0;
}
#endif

void kernel_main(void) {
	printk("Thank you for using SlugOS\n");
	#ifdef DEBUG
	// test liballoc
	liballoc_test();
	TraceStackTrace(10);
	#endif
}
