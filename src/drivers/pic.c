#include <port.h>  // Provides inb, outb, and io_wait

// PIC I/O ports
#define PIC1_CMD 0x20   // Command port for PIC1
#define PIC1_DATA 0x21  // Data port for PIC1
#define PIC2_CMD 0xA0   // Command port for PIC2
#define PIC2_DATA 0xA1  // Data port for PIC2

// PIC initialization and remapping constants
#define PIC1_OFFSET 0x20    // IRQ0-7 mapped to 0x20-0x27
#define PIC2_OFFSET 0x28    // IRQ8-15 mapped to 0x28-0x2F
#define PIC1_MASK 0xFF      // Mask for PIC1 to disable all interrupts initially
#define PIC2_MASK 0xFF      // Mask for PIC2 to disable all interrupts initially

/**
 * Sends a command byte to the PIC control register.
 * 
 * @param command Command byte to send to the PIC.
 */
static void pic_send_command(uint8_t command) {
    outb(command, PIC1_CMD);  // PIC1 command port
    io_wait();
}

/**
 * Initializes and remaps the PIC (Programmable Interrupt Controller).
 * Unmasks IRQ0 (PIT interrupt) and disables all other interrupts.
 */
void pic_init(void) {
    // Initialize PIC1
    outb(0x11, PIC1_CMD);  // Start initialization sequence for PIC1
    io_wait();
    outb(PIC1_OFFSET, PIC1_DATA);  // Set base interrupt vector for PIC1 (IRQ0 -> 0x20)
    io_wait();
    outb(0x04, PIC1_DATA);  // PIC1 has a slave at IRQ2
    io_wait();
    outb(0x01, PIC1_DATA);  // Set to 8086/88 mode
    io_wait();

    // Initialize PIC2
    outb(0x11, PIC2_CMD);  // Start initialization sequence for PIC2
    io_wait();
    outb(PIC2_OFFSET, PIC2_DATA);  // Set base interrupt vector for PIC2 (IRQ8 -> 0x28)
    io_wait();
    outb(0x02, PIC2_DATA);  // PIC2 has a master at IRQ2
    io_wait();
    outb(0x01, PIC2_DATA);  // Set to 8086/88 mode
    io_wait();

    // Mask all IRQs except IRQ0 (PIT)
    outb(0xFD, PIC1_DATA);  // Unmask IRQ0, mask all other IRQs on PIC1
    outb(0xFF, PIC2_DATA);  // Mask all IRQs on PIC2
}

/**
 * Sends an "End of Interrupt" (EOI) signal to the PIC.
 * 
 * @param irq The IRQ line to acknowledge (0 for PIC1, 1 for PIC2).
 */
void pic_end_of_interrupt(int irq) {
    if (irq >= 8) {
        outb(0x20, PIC2_CMD);  // Send EOI to PIC2 for IRQ8-15
        io_wait();
    }
    outb(0x20, PIC1_CMD);  // Send EOI to PIC1 for IRQ0-7
    io_wait();
}
