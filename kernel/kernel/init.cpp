#include <drivers/vga.h>
#include <slug.h>

extern "C" void install_gdt();
extern "C" void pic_init();
extern "C" void idt_init();

void initalize() {
    // Setup VGA text mode so printk and other functions work
    terminal_initialize();
    // Setup the GDT this code allows us to use interrupts
    install_gdt();
    // Setup the PIC (the thing that allows keyboard input and such)
    pic_init();
    // Setup the interrupts
    idt_init();
}