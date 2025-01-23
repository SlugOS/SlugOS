#include <stdint.h>
#include <slug.h>

// Send the end of interrupt to the PIC
void PIC_sendEOI(uint8_t irq);

// The PS/2 keyboard handler
void keyhandler() {
    uint8_t scancode = inb(0x60);
    printk("Scancode %x\n", scancode);
    PIC_sendEOI(1); // Send IRQ 1 EOI (IRQ1 is the PS/2 keyboard)
}