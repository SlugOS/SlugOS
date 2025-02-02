#include <drivers/vga.h>
#include <drivers/serial.h>
#include <slug.h>

void pic_disable(void);
void PIC_remap(int offset1, int offset2);
void IRQ_clear_mask(uint8_t IRQline);
void idt_init();
void heap_init();

void initialize() {
    // Setup VGA text mode so printk and other functions work
    terminal_initialize();
    // Setup serial communication for the debug shell
    init_serial();
    // Disable all of the PIC interrupts
    pic_disable();
    // Remap the PIC
    PIC_remap(0x20, 0x2F);
    // Unmask the keyboard
    IRQ_clear_mask(1);
    // Since we already setup the GDT we will now setup interrupts
    idt_init();
    // Setup the heap
    heap_init();
}