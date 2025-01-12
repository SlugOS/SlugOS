#include <slug.h>

__attribute__((noreturn))
void exception_handler() {
    writestring("Exception handler triggered\n");
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
    while(1);
}