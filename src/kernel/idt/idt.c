#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Function prototype for remapping PICs
void PIC_remap(int offset1, int offset2);

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;

#define IDT_MAX_DESCRIPTORS 256

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance
static idtr_t idtr;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08;
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

static bool vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[];

void test() {
    printf("Hello, interrupt world!\n");
    return;
}

void idt_init() {
    // Remap the PICs to use 0x20 for the master PIC and 0x28 for the slave PIC
    PIC_remap(0x20, 0x28);
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    // Initialize interrupts 0x00 - 0x1f (exceptions)
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E); // Set interrupt gates
        vectors[vector] = true;
    }

    // Set up the int $80 call
    idt_set_descriptor(0x80, test, 0x8E); // System call interrupt handler
    __asm__ volatile ("lidt %0" : : "m"(idtr)); // Load the new IDT
    __asm__ volatile ("sti"); // Set the interrupt flag
}