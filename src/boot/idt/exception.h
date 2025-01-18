#pragma once

#include <stdint.h>

typedef struct interrupt_frame_t {
    uint32_t int_num;  // Interrupt number
    uint32_t eax;      // Register values (General-purpose registers)
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t esp;      // Stack pointer at the time of the interrupt
    uint32_t eip;      // Instruction pointer at the time of the interrupt
    uint32_t eflags;   // Flags register
} interrupt_frame_t;
