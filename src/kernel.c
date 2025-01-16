#include <slug.h>
#include "test/test.h"

void kernel_main(void) {
    #ifdef DEBUG
    klog(2, "Debugging mode enabled. This is not reccomended for user builds only for development\n");
    #endif
    writestring("Thank you for using SlugOS!\n");
    #ifdef DEBUG
    // Run our test interrupt handler
    asm("int $80");
    // Run the co-op multitasking testp
    multitasking_test();
    #endif
}