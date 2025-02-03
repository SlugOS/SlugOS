// This code will define the different error codes for each architecture.

#ifndef CODE_H
#define CODE_H

const char* errorcode[] = {
    "DIVISION ERROR",            // 0
    "DEBUG",                     // 1
    "NON-MASKABLE INTERRUPT",    // 2
    "BREAKPOINT",                // 3
    "OVERFLOW",                  // 4
    "BOUND RANGE EXCEEDED",      // 5
    "INVALID OPCODE",            // 6
    "DEVICE NOT AVAILABLE",      // 7
    "DOUBLE FAULT",              // 8
    "COPROCESSOR SEGMENT OVERRUN", // 9
    "INVALID TSS",               // 10
    "SEGMENT NOT PRESENT",       // 11
    "STACK-SEGMENT FAULT",       // 12
    "GENERAL PROTECTION FAULT",  // 13
    "PAGE FAULT",                // 14
    "RESERVED",                  // 15
    "x87 FLOATING-POINT EXCEPTION", // 16
    "ALIGNMENT CHECK",           // 17
    "MACHINE CHECK",             // 18
    "SIMD FLOATING-POINT EXCEPTION", // 19
    "VIRTUALIZATION EXCEPTION",  // 20
    "CONTROL PROTECTION EXCEPTION", // 21
    "RESERVED",                  // 22
    "RESERVED",                  // 23
    "RESERVED",                  // 24
    "RESERVED",                  // 25
    "HYPERVISOR INJECTION EXCEPTION", // 26
    "VMM COMMUNICATION EXCEPTION", // 27
    "SECURITY EXCEPTION",        // 28
    "RESERVED",                  // 29
    "FPU ERROR INTERRUPT",       // 30
    // These are issues that are non CPU exceptions like memory errors (such as SSP)
    "SSP VIOLATION",             // 31
    "KERNEL PANIC (GENERIC)",    // 32
    "WATCHDOG TIMEOUT",          // 33
};

#endif