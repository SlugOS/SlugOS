#include <slug.h>

void kernel_main(void) {
    writestring("Thank you for using SlugOS!\n");
    asm("int $80");
}