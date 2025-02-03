#include <drivers/io.h>  // For outb and I/O operations
#include <stdint.h>      // For uint8_t, uint16_t, etc.
#include <slug.h>

#define PIT_CONTROL_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define IRQ0_HANDLER_PORT 0x20  // Typically 0x20 for PIC1 EOI

volatile uint32_t tick_count = 0;  // System tick counter, incremented every IRQ0 interrupt
volatile uint32_t watchdog_counter = 0;  // Watchdog counter to track watchdog timeout

// IRQ0 handler function
void IRQ0_handler() {
    tick_count++;  // Increment the system tick counter

    // Reset the watchdog timer (kick the watchdog)
    watchdog_counter = 0;

    // Send End-of-Interrupt (EOI) signal to the PIC (Programmable Interrupt Controller)
    outb(IRQ0_HANDLER_PORT, 0x20);  // EOI signal for PIC1 (master PIC)
}

void pit_init() {
    // Set the control word for periodic interrupt mode on channel 0
    uint8_t control_word = 0x36;  // Mode 3, binary, no additional options
    outb(PIT_CONTROL_PORT, control_word); // Send to control port

    // Set the divisor for 1ms (1193)
    uint16_t divisor = 1193; // 1.19318 MHz / 1000 = 1193

    // Split the divisor into low and high bytes
    uint8_t low_byte = divisor & 0xFF;
    uint8_t high_byte = (divisor >> 8) & 0xFF;

    // Send the low and high bytes to the PIT channel 0 data port
    outb(PIT_CHANNEL0_PORT, low_byte);  // Send low byte
    outb(PIT_CHANNEL0_PORT, high_byte); // Send high byte
}
