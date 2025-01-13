#include <slug.h>
#include <liballoc.h>
#include <stdbool.h>
#include <string.h>

void PIC_remap(int offset1, int offset2);
void terminal_initialize(void);
void init_cpu(void);
void idt_init(void);
int init_serial();

bool serial_status = false;

void _init() {
    // High priority code (needed for the OS to function)
    terminal_initialize();
    klog(1, "VGA text mode has been initialized\n");
	init_cpu();
    klog(1, "GDT and TSS have been initialized\n");
    PIC_remap(0x20, 0x2F);
    klog(1, "PIC has been remapped\n");
	idt_init();
    klog(1, "IDT has been initialized\n");
    // Code for testing Liballoc
    void* ptr = malloc(6);
    memcpy(ptr, "test\n", 6);
    if (strcmp(ptr, "test\n") == 0) {
        klog(1, "Liballoc test succeeded\n");
    } else {
        klog(4, "Liballoc test failed\n");
    }
    free(ptr);
    // Low priority code (stuff like serial initilization)
    if (init_serial() == 1) {
        klog(2, "Serial port is faulty, the serial port will not be able to send data");
        serial_status = false;
    } else {
        klog(1, "Serial port has been initialized\n");
        serial_status = true;
    }
}