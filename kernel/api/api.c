#include <slug.h>
#include <stdint.h>

typedef struct {
    uint32_t edi;   // General-purpose registers
    uint32_t esi;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx
    uint32_t eip;   // Instruction pointer (Program Counter)
    uint32_t cs;    // Code segment (for context switching)
    uint32_t eflags;  // CPU flags (for state restoration)

    uint32_t esp;   // Stack pointer
    uint32_t ss;    // Stack segment (for context switching)

    // Interrupt or exception specific info
    uint32_t interrupt_number; // ID of the interrupt or exception
    uint32_t error_code; // Error code, for exceptions like page faults (may be 0 otherwise)
} interrupt_stack_frame_t;

void api(interrupt_stack_frame_t* frame) {
    printk("API called from userspace!\n");
}