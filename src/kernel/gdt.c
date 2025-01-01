#include <stdint.h>

// Define a GDT entry structure
struct GDTEntry {
    uint16_t limit_low;  // Lower 16 bits of the segment limit
    uint16_t base_low;   // Lower 16 bits of the base address
    uint8_t base_middle; // Next 8 bits of the base address
    uint8_t access;      // Access byte
    uint8_t granularity; // High nibble of limit and flags
    uint8_t base_high;   // Last 8 bits of the base address
} __attribute__((packed));

// Define a GDT pointer structure
struct GDTPtr {
    uint16_t limit; // Size of the GDT
    uint32_t base;  // Address of the first GDTEntry
} __attribute__((packed));

// Declare GDT entries
struct GDTEntry gdt[4];

// Declare the GDT pointer
struct GDTPtr gdt_ptr;

// Task State Segment (TSS)
struct TSSEntry {
    uint32_t reserved1;
    uint64_t rsp[3];  // Stack pointers (RSP0, RSP1, RSP2)
    uint64_t reserved2;
    uint64_t ist[7];  // Interrupt Stack Table
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t iopb_offset;
} __attribute__((packed));

// Declare a TSS instance
struct TSSEntry tss = {0};

// Function to set a GDT entry
void set_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;
    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].granularity = (limit >> 16) & 0x0F | (flags << 4);
    gdt[index].access = access;
}

// Function to initialize the GDT
void gdt_init() {
    // Null Descriptor
    set_gdt_entry(0, 0, 0x00000000, 0x00, 0x0);

    // Kernel Mode Code Segment
    set_gdt_entry(1, 0x00400000, 0x003FFFFF, 0x9A, 0xC);

    // Kernel Mode Data Segment
    set_gdt_entry(2, 0x00800000, 0x003FFFFF, 0x92, 0xC);

    // Task State Segment
    set_gdt_entry(3, (uint32_t)&tss, sizeof(tss) - 1, 0x89, 0x0);

    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    // Load the GDT
    asm volatile ("lgdt (%0)" : : "r" (&gdt_ptr));

    // Load the TSS
    asm volatile (
        "ltr %0" 
        : 
        : "r" ((uint16_t)(3 << 3))  // GDT index 3 for TSS, shifted to form a selector
    );
}
