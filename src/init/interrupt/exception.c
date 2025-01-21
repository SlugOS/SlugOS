#include <slug.h>

__attribute__((noreturn))
void exception_handler() {
    printk("Exception handler triggered\n");
    #ifdef DEBUG
    // Do a stack trace
    TraceStackTrace(10);
    #endif
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
