#include <drivers/vga.h>

void pic_disable(void);
void idt_init();

void initialize() {
    // Setup VGA text mode so printk and other functions work
    terminal_initialize();
    // Mask all of the PIC interrupts for now
    pic_disable();
    // Since we already setup the GDT we will now setup interrupts
    idt_init();
}