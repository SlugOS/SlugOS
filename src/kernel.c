#include <slug.h>

void kernel_main(void) {
    writestring("Hello, World!\n");
    //asm("int $1");
}