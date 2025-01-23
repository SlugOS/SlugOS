#include <drivers/vga.h>
#include <slug.h>

extern "C" void install_gdt();
extern "C" void idt_init();

void initalize() {
    // Setup VGA text mode so printk and other functions work
    terminal_initialize();
    // Setup the GDT this code allows us to use interrupts
    install_gdt();
    // Setup the interrupts
    idt_init();
}