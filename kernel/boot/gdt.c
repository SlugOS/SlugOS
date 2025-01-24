#include <stdint.h>

extern uint32_t interrupt_stack_top;

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

// TSS structure
struct tss_entry {
    uint32_t prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
    uint32_t esp0;       // The stack pointer to load when we change to kernel mode.
    uint32_t ss0;        // The stack segment to load when we change to kernel mode.
    uint32_t esp1;       // Unused...
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;         
    uint32_t cs;        
    uint32_t ss;        
    uint32_t ds;        
    uint32_t fs;       
    uint32_t gs;         
    uint32_t ldt;      
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));

// Define the GDT with 6 descriptors
struct gdt_entry gdt[6];

// GDT pointer to be loaded into the GDTR register
struct gdt_ptr gdtp;

// TSS
struct tss_entry tss;

void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= granularity & 0xF0;
    gdt[num].access = access;
}

void setup_tss(uint32_t kernel_stack) {
    uint32_t base = (uint32_t)&tss;
    uint32_t limit = sizeof(tss);

    // Set up the TSS entry in the GDT
    set_gdt_entry(5, base, limit, 0x89, 0x00);

    // Zero out the TSS
    for (int i = 0; i < sizeof(tss); i++) {
        ((uint8_t*)&tss)[i] = 0;
    }

    tss.ss0 = 0x10;  // Kernel data segment
    tss.esp0 = kernel_stack;  // Set the kernel stack pointer

    // Set the I/O permission bitmap to the end of the TSS
    tss.iomap_base = sizeof(tss);
}

void install_gdt() {
    // Set the GDT pointer
    gdtp.limit = (sizeof(struct gdt_entry) * 6) - 1; // Size of GDT is 6 entries
    gdtp.base = (uint32_t)&gdt;  // Base address of the GDT

    // Set up the GDT entries
    set_gdt_entry(0, 0, 0, 0, 0);               // NULL descriptor
    set_gdt_entry(1, 0, 0xFFFFF, 0x9A, 0xCF); // Kernel code segment
    set_gdt_entry(2, 0, 0xFFFFF, 0x92, 0xCF); // Kernel data segment
    set_gdt_entry(3, 0, 0xFFFFF, 0xFA, 0xCF); // User code segment
    set_gdt_entry(4, 0, 0xFFFFF, 0xF2, 0xCF); // User data segment

    setup_tss(&interrupt_stack_top); // The stack is in boot.s and is not the one the kernel uses but a different one which is not used yet

    // Load the GDT
    asm volatile("lgdt %0" : : "m"(gdtp));

    // Load the TSS
    asm volatile("ltr %%ax" : : "a"(0x28)); // 0x28 is the offset of the TSS in the GDT (5 * 8)
}