#include <slug.h>
#include <liballoc.h>
#include <string.h>

void kernel_main(void) {
    writestring("Thank you for using SlugOS!\n");
    /* asm("int $1"); */ // This code is a test for the interrupt handler
}