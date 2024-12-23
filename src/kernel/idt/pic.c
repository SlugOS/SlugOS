#include <port.h>  // Assuming this provides 'inb', 'outb', and 'io_wait'

// PIC Control Ports
#define PIC1_COMMAND 0x20       // Master PIC command port
#define PIC1_DATA    0x21       // Master PIC data port
#define PIC2_COMMAND 0xA0       // Slave PIC command port
#define PIC2_DATA    0xA1       // Slave PIC data port

/* PIC Initialization Command Words (ICWs) */
#define ICW1_ICW4       0x01    // Indicates that ICW4 will be present
#define ICW1_SINGLE     0x02    // Single (cascade) mode
#define ICW1_INTERVAL4  0x04    // Call address interval 4 (8)
#define ICW1_LEVEL      0x08    // Level triggered (edge) mode
#define ICW1_INIT       0x10    // Initialization - required!

#define ICW4_8086       0x01    // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO       0x02    // Auto (normal) EOI
#define ICW4_BUF_SLAVE  0x08    // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C    // Buffered mode/master
#define ICW4_SFNM       0x10    // Special fully nested (not) mode

/*
Arguments:
    offset1 - vector offset for master PIC
        Vectors on the master become offset1..offset1+7
    offset2 - vector offset for slave PIC:
        Vectors on the slave become offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2) {
    uint8_t a1, a2;

    // Step 1: Save the current interrupt masks
    a1 = inb(PIC1_DATA);  // Read current mask for master PIC
    a2 = inb(PIC2_DATA);  // Read current mask for slave PIC

    // Step 2: Begin initialization sequence (both PICs must receive ICW1 with ICW4 enabled)
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // Starts the initialization sequence (master)
    io_wait();                                   // Wait for a brief moment to complete ICW1
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);  // Starts the initialization sequence (slave)
    io_wait();                                   // Wait for slave PIC

    // Step 3: Set vector offset for both master and slave PICs
    outb(PIC1_DATA, offset1);  // Set vector offset for master PIC
    io_wait();                  // Wait to complete the writing of the offset
    outb(PIC2_DATA, offset2);  // Set vector offset for slave PIC
    io_wait();                  // Wait to complete the writing of the offset

    // Step 4: Configure master PIC to point to slave PIC on IRQ2
    outb(PIC1_DATA, 4);        // Tell master PIC that slave is on IRQ2 (bit 2 -> 0000 0100)
    io_wait();
    outb(PIC2_DATA, 2);        // Tell slave PIC its cascade identity (IRQ2 of master -> slave pic's IRQ0)
    io_wait();

    // Step 5: Set PICs to 8086 mode (for compatibility with x86 systems)
    outb(PIC1_DATA, ICW4_8086);  // Set the master PIC to 8086 mode
    io_wait();
    outb(PIC2_DATA, ICW4_8086);  // Set the slave PIC to 8086 mode
    io_wait();

    // Step 6: Restore the saved masks (to not disable any interrupts)
    outb(PIC1_DATA, a1);  // Restore master PIC interrupt mask
    outb(PIC2_DATA, a2);  // Restore slave PIC interrupt mask
}
