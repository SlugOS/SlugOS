#include <slug.h>
#include <stdint.h>

// This contains all of the registers and other info that is passed from the interrupt
typedef struct {
    uint32_t ds;       // Data segment selector
    uint32_t edi, esi, ebp, ebx, edx, ecx, eax; // General-purpose registers
    int int_num; // The interrupt number
    uint32_t err_code; // Some exceptions pass this
    uint32_t eip;      // Instruction pointer at the moment of the exception
    uint32_t cs;       // Code segment at the moment of the exception
    uint32_t eflags;   // Flags register
    uint32_t esp;
    uint32_t ss;       // Stack segment (if applicable)
} exception_frame_t;

// Every type of exception that can happen
const char* exception_messages[] = {
    "Divide By Zero",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved",
    "FPU Error Interrupt",
};

const char* get_exception_message(uint32_t int_num) {
    if (int_num < sizeof(exception_messages) / sizeof(exception_messages[0])) {
        return exception_messages[int_num];
    } else {
        return "Unknown Exception";
    }
}

__attribute__((noreturn))
void exception_handler(exception_frame_t* frame) {
    int int_num = frame->int_num;
    char* error = get_exception_message(int_num);
    printk("Exception handler triggered: %s\n Exception %d\n", error, int_num);
    #ifdef DEBUG
    // Do a stack trace
    TraceStackTrace(10);
    #endif
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
