#include <stdint.h>

#pragma pack(push, 1)
extern void* TSS_ADDRESS;  // TSS address is now a pointer
extern uint32_t TSS_SIZE;  // TSS size is still a uint32_t

// GDT Descriptor structure
typedef struct {
    uint16_t limit_low;      // Lower 16 bits of the limit
    uint16_t base_low;       // Lower 16 bits of the base
    uint8_t base_middle;     // Next 8 bits of the base
    uint8_t access;          // Access byte
    uint8_t flags;           // Flags
    uint8_t base_high;       // Highest 8 bits of the base
} __attribute__((packed)) GDTEntry;

// GDT Pointer structure
typedef struct {
    uint16_t limit;          // The upper 16 bits of all selector limits
    uint32_t base;           // The address of the first GDT Entry
} __attribute__((packed)) GDTPtr;

// External functions (or placeholder) for TSS and its size
extern void* TSS_ADDRESS; // This should point to the actual Task State Segment (TSS)
extern uint32_t TSS_SIZE;  // Size of the TSS


// GDT Entries Declaration
GDTEntry gdt[6];
GDTPtr gdtr;

// Function to load the GDT
void load_gdt(GDTPtr* gdtr) {
    asm volatile(
        "lgdt (%0)"         // Load the GDT from the given address
        : : "r" (gdtr)
    );
}

// Initialize the GDT with the entries
void init_gdt() {
    // Null Descriptor (Index 0)
    gdt[0] = (GDTEntry){0};

    // Kernel Mode Code Segment (Index 1)
    gdt[1] = (GDTEntry){
        .limit_low = 0xFFFF,
        .base_low = 0x0000,
        .base_middle = 0x00,
        .access = 0x9A,  // Present, Ring 0, Code Segment
        .flags = 0xC,    // Granularity 4K, 32-bit
        .base_high = 0x00
    };

    // Kernel Mode Data Segment (Index 2)
    gdt[2] = (GDTEntry){
        .limit_low = 0xFFFF,
        .base_low = 0x0000,
        .base_middle = 0x00,
        .access = 0x92,  // Present, Ring 0, Data Segment
        .flags = 0xC,    // Granularity 4K, 32-bit
        .base_high = 0x00
    };

    // User Mode Code Segment (Index 3)
    gdt[3] = (GDTEntry){
        .limit_low = 0xFFFF,
        .base_low = 0x0000,
        .base_middle = 0x00,
        .access = 0xFA,  // Present, Ring 3, Code Segment
        .flags = 0xC,    // Granularity 4K, 32-bit
        .base_high = 0x00
    };

    // User Mode Data Segment (Index 4)
    gdt[4] = (GDTEntry){
        .limit_low = 0xFFFF,
        .base_low = 0x0000,
        .base_middle = 0x00,
        .access = 0xF2,  // Present, Ring 3, Data Segment
        .flags = 0xC,    // Granularity 4K, 32-bit
        .base_high = 0x00
    };

    // Task State Segment (TSS) Descriptor (Index 5)
    gdt[5] = (GDTEntry){
        .limit_low = TSS_SIZE - 1,
        .base_low = (uint32_t)TSS_ADDRESS & 0xFFFF,
        .base_middle = ((uint32_t)TSS_ADDRESS >> 16) & 0xFF,
        .access = 0x89,  // Present, System Segment (TSS)
        .flags = 0x0,    // No flags
        .base_high = ((uint32_t)TSS_ADDRESS >> 24) & 0xFF
    };

    // Set up the GDTR (Global Descriptor Table Register)
    gdtr.limit = sizeof(gdt) - 1;     // Size of the GDT (6 entries)
    gdtr.base = (uint32_t)&gdt;        // Base address of the GDT

    // Load the GDT and update the code segment registers (with the IDT register)
    load_gdt(&gdtr);
}
