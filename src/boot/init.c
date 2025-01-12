#include <slug.h>
#include <stdbool.h>

void PIC_remap(int offset1, int offset2);
void terminal_initialize(void);
void init_cpu(void);
void idt_init(void);
int init_serial();

bool serial_status = false;

void _init() {
    // High priority code (needed for the OS to function)
    terminal_initialize();
    // Low priority code (stuff like serial initilization)
    if (init_serial() == 1) {
        klog(2, "Serial port is faulty, the serial port will not be able to send data");
        serial_status = false;
    } else {
        serial_status = true;
    }
    klog(1, "Terminal initialized\n");
	init_cpu();
    klog(1, "CPU has been initialized\n");
    PIC_remap(0x20, 0x2F);
    klog(1, "PIC has been remapped\n");
	idt_init();
    klog(1, "IDT has been initialized\n");
}