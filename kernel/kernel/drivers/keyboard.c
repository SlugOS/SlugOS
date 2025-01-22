#include <slug.h>
#include <drivers/io.h>

extern void PIC_sendEOI(uint8_t irq);

void keyboard_handler() {
    printk("Keyboard handler called");
    uint8_t scancode = inb(0x60);
    PIC_sendEOI(1);
    return;
}