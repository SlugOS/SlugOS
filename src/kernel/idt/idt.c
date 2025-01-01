#include <stdbool.h>
#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

typedef struct {
    uint16_t    isr_low;      // Lower 16 bits of the ISR address
    uint16_t    kernel_cs;    // GDT segment selector loaded into CS before calling the ISR
    uint8_t     reserved;     // Set to zero
    uint8_t     attributes;   // Type and attributes; see the IDT page
    uint16_t    isr_high;     // Higher 16 bits of the ISR address
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t    limit;
    uint32_t    base;
} __attribute__((packed)) idtr_t;

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];  // IDT array, aligned for performance
static idtr_t idtr;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // GDT offset for kernel code segment
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

// To track which entries are used (for example, enabling IRQs after initialization)
static bool vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[];

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];  // Base address of the IDT
    idtr.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1);  // IDT size limit

    // Initialize standard exception vectors (0-31)
    for (uint8_t vector = 0; vector < 31; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);  // 0x8E: Interrupt gate with privileges and present
        vectors[vector] = true;
    }

    // Load IDT into CPU via `lidt` instruction
    __asm__ volatile ("lidt %0" : : "m"(idtr));
    __asm__ volatile ("sti");  // Enable interrupts globally after IDT is set
}
