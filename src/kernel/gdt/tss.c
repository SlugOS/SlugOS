#include <stdint.h>

// Define the Task State Segment (TSS) structure
typedef struct {
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    // etc.
} TSS;

// Declare and define TSS structure
TSS tss __attribute__((aligned(0x1000)));  // Ensuring it’s aligned to a 4KB boundary

// Use pointer for TSS_ADDRESS
void* TSS_ADDRESS = &tss;  // TSS pointer
uint32_t TSS_SIZE = sizeof(tss);  // Size of the TSS
