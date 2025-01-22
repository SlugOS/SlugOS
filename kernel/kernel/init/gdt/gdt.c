#include <stdint.h>

#define GDT_SIZE 4 // Increase size to accommodate null, code, data, and TSS descriptors

// GDT Entry Structure
struct gdt_entry {
    uint16_t limit_low;    // Lower 16 bits of limit
    uint16_t base_low;     // Lower 16 bits of base
    uint8_t base_middle;   // Next 8 bits of base
    uint8_t access;        // Access byte
    uint8_t granularity;   // Granularity and upper 4 bits of limit
    uint8_t base_high;     // Upper 8 bits of base
} __attribute__((packed));

// GDT Pointer Structure
struct gdt_ptr {
    uint16_t limit;   // Size of GDT - 1
    uint32_t base;    // Address of the first GDT entry
} __attribute__((packed));

// GDT and pointer
struct gdt_entry gdt[GDT_SIZE];
struct gdt_ptr gp;

// Function to set up a GDT entry
void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].access = access;
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
    gdt[num].base_high = (base >> 24) & 0xFF;
}

// Initialize the GDT
void gdt_init() {
    // Null descriptor
    set_gdt_entry(0, 0, 0, 0x00, 0x00);

    // Kernel code segment (base = 0x0, limit = 4GB, access = executable, readable)
    set_gdt_entry(1, 0x00000000, 0xFFFFFFFF, 0x9A, 0xCF);

    // Kernel data segment (base = 0x0, limit = 4GB, access = writable)
    set_gdt_entry(2, 0x00000000, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the GDT pointer
    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    __asm__ __volatile__("lgdt (%0)" : : "r"(&gp));
}