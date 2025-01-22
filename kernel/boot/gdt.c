#include <stdint.h>

// GDT entry structure
struct gdt_entry {
    uint16_t limit_low;       // Lower 16 bits of the limit
    uint16_t base_low;        // Lower 16 bits of the base
    uint8_t base_middle;      // Next 8 bits of the base
    uint8_t access;           // Access flags
    uint8_t granularity;      // Granularity and upper 4 bits of the limit
    uint8_t base_high;        // Upper 8 bits of the base
} __attribute__((packed));

// GDT pointer structure
struct gdt_ptr {
    uint16_t limit;           // Size of the GDT
    uint32_t base;            // Address of the GDT
} __attribute__((packed));

// Define the GDT with only 3 descriptors
struct gdt_entry gdt[3];

// GDT pointer to be loaded into the GDTR register
struct gdt_ptr gdtp;

void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= granularity & 0xF0;
    gdt[num].access = access;
}

void install_gdt() {
    // Set the GDT pointer
    gdtp.limit = (sizeof(struct gdt_entry) * 3) - 1; // Size of GDT is 3 entries
    gdtp.base = (uint32_t)&gdt;  // Base address of the GDT

    // Set up the three GDT entries
    set_gdt_entry(0, 0, 0, 0, 0);               // NULL descriptor
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment (accessible for kernel code)
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment (accessible for kernel data)

    // Load the GDT (GDTR contains the GDT pointer)
    asm volatile("lgdt %0" : : "m"(gdtp));
}
