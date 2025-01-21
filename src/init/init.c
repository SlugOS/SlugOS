#include <slug.h>

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
    // Disable the PIC so we can use the APIC
    pic_disable();
    logk(1, "PIC disabled");
    // Setup the IDT
    idt_init();
    logk(1, "IDT enabled");
    return;
}