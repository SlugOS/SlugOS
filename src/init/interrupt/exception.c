#include <slug.h>
#include <stdint.h>

// This contains all of the registers and other info that is passed from the interrupt
typedef struct {
    int int_num;
    uint32_t ss;
    uint32_t esp;
    uint32_t eflags;
    uint16_t cs;
    uint32_t eip;
    uint32_t error_code;
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
void exception_handler(int int_num) {
    char* error = get_exception_message(int_num);
    printk("Exception handler triggered: %s\n Exception %d\n", error, int_num);
    #ifdef DEBUG
    // Do a stack trace
    TraceStackTrace(10);
    #endif
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}
