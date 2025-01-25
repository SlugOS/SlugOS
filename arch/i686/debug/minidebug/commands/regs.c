/*
Dump the registers
*/

#include <stdint.h>
#include "common.h"

void dump_registers_serial() {
    uint32_t eax, ebx, ecx, edx, esi, edi, ebp, esp, eip;
    char buffer[32]; // Sufficient for label + value + newline

    // Collect register values
    asm volatile("mov %%eax, %0" : "=r"(eax));
    asm volatile("mov %%ebx, %0" : "=r"(ebx));
    asm volatile("mov %%ecx, %0" : "=r"(ecx));
    asm volatile("mov %%edx, %0" : "=r"(edx));
    asm volatile("mov %%esi, %0" : "=r"(esi));
    asm volatile("mov %%edi, %0" : "=r"(edi));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("call 1f\n"
                 "1: pop %0"
                 : "=r"(eip)); // Get EIP manually

    // Format and send each register's value
    puts_serial("EAX: ");
    int_to_hex(eax, buffer);
    puts_serial(buffer);
    puts_serial("\n");

    puts_serial("EBX: ");
    int_to_hex(ebx, buffer);
    puts_serial(buffer);
    puts_serial("\n");

    puts_serial("ECX: ");
    int_to_hex(ecx, buffer);
    puts_serial(buffer);
    puts_serial("\n");

    puts_serial("EDX: ");
    int_to_hex(edx, buffer);
    puts_serial(buffer);
    puts_serial("\n");

    puts_serial("ESI: ");
    int_to_hex(esi, buffer);
    puts_serial(buffer);
    puts_serial("\n");

    puts_serial("EDI: ");
    int_to_hex(edi, buffer);
    puts_serial(buffer);
    puts_serial("\n");

    puts_serial("EBP: ");
    int_to_hex(ebp, buffer);
    puts_serial(buffer);
    puts_serial("\n");

    puts_serial("ESP: ");
    int_to_hex(esp, buffer);
    puts_serial(buffer);
    puts_serial("\n");

    puts_serial("EIP: ");
    int_to_hex(eip, buffer);
    puts_serial(buffer);
    puts_serial("\n");
}
