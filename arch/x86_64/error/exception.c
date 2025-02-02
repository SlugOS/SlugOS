#include <slug.h>
#include <stdint.h>

struct interrupt_frame {
    // Pushed by our ISR stub
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    // Pushed by CPU
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

const char* get_error_message(int error_number);

void recover(uint64_t interrupt_number) {
    #ifdef DEBUG
    printk("Recoverable error %s\n", get_error_message(interrupt_number));
    #endif
    return;
}

__attribute__((noreturn))
void exception_handler(uint64_t interrupt_number, struct interrupt_frame* frame) {
    // Now we have access to both the interrupt number and the complete CPU state
    crash(interrupt_number);
}

void exception_handler_recoverable(uint64_t interrupt_number, struct interrupt_frame* frame) {
    // Now we have access to both the interrupt number and the complete CPU state
    recover(interrupt_number);
}