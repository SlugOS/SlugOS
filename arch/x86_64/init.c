#include <drivers/vga.h>
#include <drivers/serial.h>
#include <slug.h>

// Function declarations
void pic_disable(void);
void pit_init(void);
void PIC_remap(int offset1, int offset2);
void IRQ_clear_mask(uint8_t IRQline);
void idt_init(void);
void heap_init(void);
void jump_usermode(void);
void scheduler_init(void);

// Initializes core system components
void initialize(void) {
    // Display subsystem initialization
    terminal_initialize();
    add_to_log("VGA initialized");

    // Serial subsystem initialization
    if (init_serial() != 0) {
        add_to_log("Serial initialization failed");
    } else {
        add_to_log("Serial initialized");
    }

    // Interrupt handling setup
    pic_disable();
    pit_init();
    PIC_remap(0x20, 0x2F);
    
    // Unmask necessary interrupt lines
    IRQ_clear_mask(0);    // PIT
    IRQ_clear_mask(1);    // Keyboard
    
    add_to_log("PIT initialized");
    add_to_log("PIC remapped");
    add_to_log("Interrupt lines unmasked");

    // IDT initialization
    idt_init();
    add_to_log("IDT initialized");

    // Heap initialization
    heap_init();
    add_to_log("Heap initialized");

    // Scheduler initialization
    scheduler_init();
    add_to_log("Scheduler initialized");
}