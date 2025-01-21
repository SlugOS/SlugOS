#include <slug.h>
#include <stdint.h>

void PIC_sendEOI(uint8_t irq);
static void PIC_remap(int offset1, int offset2);
void IRQ_disable(uint8_t IRQline);
void IRQ_enable(uint8_t IRQline);

void _init() {
    // Setup the needed variables and such for the rest of the VGA text mode functions
    terminal_initialize();
    #ifdef DEBUG
    logk(2, "DEBUG mode enabled this is only reccomended for development");
    #endif
    logk(1, "Terminal initalized");
    // Setup basic segmentation using GDT (kernel code, user code etc)
    gdt_init();
    logk(1, "GDT & TSS initalized");
    // Remap the PIC needed to not cause a GPF or some other issue
    PIC_remap(0x20, 0x2F);
    logk(1, "PIC initalized");
    // Setup the IDT
    idt_init();
    logk(1, "IDT enabled");
    return;
}