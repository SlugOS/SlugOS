; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
MBFLAGS  equ  MBALIGN | MEMINFO ; Multiboot flag field
MAGIC    equ  0x1BADB002        ; magic number to help bootloader recognize header
CHECKSUM equ -(MAGIC + MBFLAGS) ; checksum to verify header validity

section .multiboot
    align 4
    dd MAGIC            ; Magic number
    dd MBFLAGS          ; Flags
    dd CHECKSUM         ; Checksum (negative of MAGIC + MBFLAGS)

section .bss
    align 16
stack_bottom:
    resb 16384          ; Reserve 16 KiB of stack space
stack_top:

section .text
    global _start

_start:
    ; Correct the stack pointer (move to the top of the allocated space)
    mov esp, stack_top  ; Set the stack pointer to the top of stack (highest address)

    ; Clear the EBP register, this will assist with a clean stack trace in case needed
    xor ebp, ebp

    ; Call the initialization function to set up global constructors
    extern _init         ; _init comes from crtbegin.o and crti.o
    call _init

    ; Now call the main kernel function after initialization
    extern kernel_main
    call kernel_main

	; Finish global constructors and deal with no longer needing them
	extern _fini
	call_fini

    cli ; disable interrupt

.hang:
    hlt                  ; Halt the CPU (infinite loop)
    jmp .hang            ; Jump back to the same instruction (infinite hang)

.end:
