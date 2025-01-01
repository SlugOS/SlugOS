#include <stdio.h>

void gdt_init();

void kernel_main() {
    gdt_init();
    printf("Thank you for using SlugOS\n");
    // Halt our kernel since nothing left to do
    asm("cli; hlt");
}