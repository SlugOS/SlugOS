#include <stdint.h>

#define GDT_SIZE 5 // Increase size to accommodate null, code, data, and TSS descriptors

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

// Task State Segment Structure
struct tss_entry {
    uint32_t prev_task_link;  // Previous task link (not used)
    uint32_t esp0;            // Stack pointer for ring 0
    uint32_t ss0;             // Stack segment selector for ring 0
    uint32_t reserved[22];    // Rest of TSS (zero-filled for simplicity)
} __attribute__((packed));

// GDT and pointer
struct gdt_entry gdt[GDT_SIZE];
struct gdt_ptr gp;

// TSS
struct tss_entry tss;

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

    // User mode code segment (base = 0x0, limit = 4GB, access = user executable)
    set_gdt_entry(3, 0x00000000, 0xFFFFFFFF, 0xFA, 0xCF);

    // User mode data segment (base = 0x0, limit = 4GB, access = user writable)
    set_gdt_entry(4, 0x00000000, 0xFFFFFFFF, 0xF2, 0xCF);

    // Load the GDT pointer
    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    __asm__ __volatile__("lgdt (%0)" : : "r"(&gp));
}

// Initialize the TSS
void init_tss() {
    void* kernel_stack = malloc(16 * 1024);  // Allocate 16KB for the kernel stack
    tss.esp0 = (uint32_t)kernel_stack + (16 * 1024);  // esp0 points to the top of the stack
    tss.ss0 = 0x10;     // Kernel data segment (GDT index 2)
    
    // Zero-fill unused parts of the TSS
    for (int i = 0; i < 22; i++) {
        ((uint32_t*)&tss.reserved)[i] = 0;
    }

    // Add TSS descriptor to GDT (index 5)
    set_gdt_entry(5, (uint32_t)&tss, sizeof(tss) - 1, 0x89, 0x40);

    // Load the TSS
    __asm__ __volatile__(
        "ltr %%ax"
        : : "a"(0x28) // GDT selector for TSS (GDT index 5, shifted left by 3 for selector encoding)
    );
}
