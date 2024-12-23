#include <vga.h>

typedef struct {
    uint32_t gs, fs, es, ds;  // Segment registers
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // General-purpose registers
    uint32_t interrupt_number; // The interrupt number that triggered the ISR
    uint32_t error_code;      // Error code (for some exceptions)
    uint32_t eip;             // Instruction pointer at the time of the interrupt
    uint32_t cs;              // Code segment
    uint32_t eflags;          // Flags register
} stack_frame_t;

__attribute__((noreturn))
void exception_handler(stack_frame_t* stack) {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
