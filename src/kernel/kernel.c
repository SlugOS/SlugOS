#include <stdio.h>

void kernel_main() {
    printf("Thank you for using SlugOS\n");
    printf("SlugOS is a public domain FOSS operating system.\n");
    // Halt our kernel since nothing left to do
    asm("cli; hlt");
}