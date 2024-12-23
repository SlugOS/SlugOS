#include <vga.h>
#include <stdio.h>

void init_gdt();
void idt_init();
void kernel_main() {
    terminal_initialize();
    init_gdt(); // Init our GDT
    idt_init(); // Init our interrupts
}