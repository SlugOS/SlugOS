#include <stdint.h>

// Define the size of the GDT (Global Descriptor Table)
#define GDT_SIZE 4

// GDT Entry Structure
struct gdt_entry {
    uint16_t limit_low;    // Lower 16 bits of limit
    uint16_t base_low;     // Lower 16 bits of base
    uint8_t base_middle;   // Next 8 bits of base
    uint8_t access;        // Access byte
    uint8_t granularity;   // Granularity and the upper 4 bits of limit
    uint8_t base_high;     // Upper 8 bits of base
} __attribute__((packed));

// GDT Pointer Structure
struct gdt_ptr {
    uint16_t limit;   // Size of GDT - 1
    uint32_t base;    // Address of the first GDT entry
} __attribute__((packed));

// Define the Task State Segment (TSS) structure
struct tss_entry {
    uint32_t prev_task_link;  // Previous task link
    uint32_t esp0;            // Stack pointer for ring 0
    uint32_t ss0;             // Stack segment for ring 0
    uint32_t esp1;            // Stack pointer for ring 1 (not used here)
    uint32_t ss1;             // Stack segment for ring 1 (not used here)
    uint32_t esp2;            // Stack pointer for ring 2 (not used here)
    uint32_t ss2;             // Stack segment for ring 2 (not used here)
    uint32_t cr3;             // Page directory base register
    uint32_t eip;             // Instruction pointer
    uint32_t flags;           // Flags register
    uint32_t eax, ecx, edx, ebx;  // General registers
    uint32_t esp, ebp, esi, edi;  // More general registers
    uint32_t es, cs, ss, ds, fs, gs;  // Segment selectors
    uint32_t ldt;             // Local Descriptor Table selector (not used here)
    uint16_t trace_bitmap;    // Trace bitmap
} __attribute__((packed));

// Declare the GDT array and pointer
struct gdt_entry gdt[GDT_SIZE];
struct gdt_ptr gp;

// Define TSS and its base
struct tss_entry tss;

// Function to set individual GDT entries
void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].limit_low = limit & 0xFFFF;               // Set the lower 16 bits of the limit
    gdt[num].base_low = base & 0xFFFF;                 // Set the lower 16 bits of the base
    gdt[num].base_middle = (base >> 16) & 0xFF;       // Set the middle 8 bits of the base
    gdt[num].access = access;                          // Set the access byte
    gdt[num].granularity = (limit >> 16) & 0x0F;      // Set the upper 4 bits of the limit
    gdt[num].granularity |= granularity & 0xF0;        // Set the granularity flags
    gdt[num].base_high = (base >> 24) & 0xFF;          // Set the upper 8 bits of the base
}

// Initialize the GDT
void gdt_init() {
    // Set up the null descriptor (index 0)
    set_gdt_entry(0, 0, 0, 0x00, 0x0);

    // Set up the Kernel Mode Code Segment (index 1)
    set_gdt_entry(1, 0x00400000, 0x003FFFFF, 0x9A, 0xCF);

    // Set up the Kernel Mode Data Segment (index 2)
    set_gdt_entry(2, 0x00800000, 0x003FFFFF, 0x92, 0xCF);

    // Set up the Task State Segment (TSS) (index 3)
    set_gdt_entry(3, (uint32_t) &tss, sizeof(tss) - 1, 0x89, 0x0);

    // Set up the GDT pointer
    gp.limit = (sizeof(gdt) - 1);  // The size of the GDT - 1
    gp.base = (uint32_t) &gdt;     // The base address of the GDT

    // Load the GDT
    load_gdt(&gp);
}

// Set up the TSS (Task State Segment)
void init_tss() {
    // We only need the basics here
    tss.ss0 = 0x10;    // Set the kernel data segment selector (you can change this based on your setup)
    tss.esp0 = 0x90000; // Some address for the stack pointer in kernel mode (you can adjust this)
    tss.cs = 0x08;     // Code segment for kernel mode (you can adjust based on your setup)
    tss.eip = 0x00000000;  // Set to any desired address for instruction pointer (this is just an example)

    // Make sure other fields of the TSS are properly set to 0 by default
    // No need for other fields since we're only working with basic task switching
}

// Function to load the GDT (Global Descriptor Table)
void load_gdt(struct gdt_ptr* gdtp) {
    __asm__ __volatile__(
        "lgdt (%0)"       // Use the pointer passed in edi to load the GDT.
        :
        : "r" (gdtp)      // Use r (register) to pass the pointer to inline assembly
        : "memory"        // Indicate that memory may be modified
    );
}
