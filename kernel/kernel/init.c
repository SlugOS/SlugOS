#include <drivers/vga.h>
#include <slug.h>

void install_gdt();
void pic_init();
void idt_init();
int init_serial();

void initialize() {
    // Initalize the serial system for future use.
    init_serial();
    // Setup VGA text mode so printk and other functions work
    terminal_initialize();
    // Setup the GDT this code allows us to use interrupts
    install_gdt();
    // Setup the PIC (the thing that allows keyboard input and such)
    pic_init();
    // Setup the interrupts
    idt_init();
}