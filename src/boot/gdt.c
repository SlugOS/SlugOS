#include <stddef.h>
#include <stdint.h>

// ==== GDT Entry Structure ====
typedef struct gdt_entry {
    uint16_t limit_low;   // Lower 16 bits of the segment limit
    uint16_t base_low;    // Lower 16 bits of the base
    uint8_t base_middle;  // Middle 8 bits of the base
    uint8_t access;       // Access flags
    uint8_t granularity;  // Flags and upper 4 bits of limit
    uint8_t base_high;    // Higher 8 bits of the base
} __attribute__((packed)) gdt_entry;

// ==== GDT Pointer Structure ====
typedef struct gdt_ptr {
    uint16_t limit;       // Limit of the GDT
    uint32_t base;        // Base address of the GDT
} __attribute__((packed)) gdt_ptr;

// ==== TSS Entry Structure ====
typedef struct tss_entry {
    uint32_t prev_tss;   // Previous TSS link (unused)
    uint32_t esp0;       // Stack pointer to load when switching to kernel mode
    uint32_t ss0;        // Stack segment to load when switching to kernel mode
    uint32_t esp1;       // Reserved
    uint32_t ss1;        // Reserved
    uint32_t esp2;       // Reserved
    uint32_t ss2;        // Reserved
    uint32_t cr3;        // Page Directory Base Register
    uint32_t eip;        // Instruction pointer
    uint32_t eflags;     // Flags register
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi; // General-purpose registers
    uint32_t es, cs, ss, ds, fs, gs; // Segment selectors
    uint32_t ldt;        // Local Descriptor Table segment selector
    uint16_t trap;       // Trap flag (set if task is switched using hardware interrupt)
    uint16_t iomap_base; // I/O Map base address
} __attribute__((packed)) tss_entry;

// Declare the GDT and pointer
gdt_entry gdt[6];
gdt_ptr gdt_desc;

// Declare the TSS
tss_entry tss;

// Define the kernel stack (32KB)
#define KERNEL_STACK_SIZE (32 * 1024)
uint8_t kernel_stack[KERNEL_STACK_SIZE];

// Function prototypes
void load_gdt();
void load_tss(uint16_t tss_segment);
void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void init_gdt();
void setup_tss(uint32_t kernel_stack, uint16_t kernel_ss);

// ==== GDT Functions ====
void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
    gdt[num].access = access;
}

void init_gdt() {
    set_gdt_entry(0, 0, 0, 0, 0);                        // Null Segment
    set_gdt_entry(1, 0, 0xFFFFF, 0x9A, 0xC0);            // Kernel Code Segment
    set_gdt_entry(2, 0, 0xFFFFF, 0x92, 0xC0);            // Kernel Data Segment
    set_gdt_entry(3, 0, 0xFFFFF, 0xFA, 0xC0);            // User Code Segment
    set_gdt_entry(4, 0, 0xFFFFF, 0xF2, 0xC0);            // User Data Segment
    set_gdt_entry(5, (uint32_t)&tss, sizeof(tss) - 1, 0x89, 0x00); // Task State Segment

    // Set up the GDT pointer
    gdt_desc.limit = sizeof(gdt) - 1;
    gdt_desc.base = (uint32_t)&gdt;

    // Load the GDT
    load_gdt();
}

// ==== TSS Functions ====
void setup_tss(uint32_t kernel_stack_addr, uint16_t kernel_ss) {
    // Clear the TSS structure
    for (size_t i = 0; i < sizeof(tss) / sizeof(uint32_t); i++) {
        ((uint32_t *)&tss)[i] = 0;
    }


    // Set up the kernel stack and segment for privilege-level changes
    tss.esp0 = kernel_stack_addr;
    tss.ss0 = kernel_ss;

    // Set I/O Map base to the end of the TSS
    tss.iomap_base = sizeof(tss);

    // Load the TSS
    load_tss(0x28); // TSS segment selector in the GDT (5th entry, index * 8 = 0x28)
}

// Assembly functions
void load_gdt() {
    asm volatile(
        "lgdt (%0)\n"
        "mov $0x10, %%ax\n"  // Kernel Data Segment (0x10 is segment selector for entry 2)
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "jmp $0x08, $.flush\n" // Jump to the Kernel Code Segment (0x08)
        ".flush:\n"
        :
        : "r"(&gdt_desc)
        : "memory"
    );
}

void load_tss(uint16_t tss_segment) {
    asm volatile("ltr %0" : : "r"(tss_segment));
}

// ==== Initialize the GDT and TSS ====
void init_cpu() {
    // Initialize the GDT and set up TSS
    init_gdt();
    setup_tss((uint32_t)&kernel_stack[KERNEL_STACK_SIZE], 0x10); // Kernel Stack top, Kernel Data Segment
}
