#include <drivers/vga.h>
#include <slug.h>

void install_gdt();
void pic_init();
void idt_init();

void initialize() {
    // Initalize the minidebug system for future use.
    minidebug_init();
    // Setup VGA text mode so printk and other functions work
    terminal_initialize();
    // Setup the GDT this code allows us to use interrupts
    install_gdt();
    // Setup the PIC (the thing that allows keyboard input and such)
    pic_init();
    // Setup the interrupts
    idt_init();
}