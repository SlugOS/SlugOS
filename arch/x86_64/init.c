#include <drivers/vga.h>
#include <drivers/serial.h>

void pic_disable(void);
void PIC_remap(int offset1, int offset2);
void idt_init();

void initialize() {
    // Setup VGA text mode so printk and other functions work
    terminal_initialize();
    // Setup serial communication for the debug shell
    init_serial();
    PIC_remap(0x20, 0x2F);
    // Mask all of the PIC interrupts for now
    pic_disable();
    // Since we already setup the GDT we will now setup interrupts
    idt_init();
}