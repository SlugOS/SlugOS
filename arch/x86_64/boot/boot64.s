[BITS 32]
global go64

; 64-bit GDT
gdt64:
    dq 0x0000000000000000 ; Null descriptor
    dq 0x00AF9A000000FFFF ; 64-bit code segment
    dq 0x00CF92000000FFFF ; 64-bit data segment

gdt64_ptr:
    dw $ - gdt64 - 1      ; GDT length
    dq gdt64              ; GDT base address

section .text
global _start
extern kernel_main

go64:
    ; Disable interrupts
    cli

    ; Load GDT
    lgdt [gdt64_ptr]

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Set up page tables (you need to implement this)
    call setup_paging

    ; Set the long mode bit in EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; Far jump to 64-bit code
    jmp 0x08:long_mode_start

[BITS 64]
long_mode_start:
    ; Update segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up stack (adjust the address as needed)
    mov rsp, 0x200000

    ; Call the C kernel main function
    extern kernel_main
    call kernel_main

    ; If we return from kernel_main, print an error message
    mov rax, 0x4f524f454f524f45
    mov [0xb8000], rax
    mov rax, 0x4f3a4f524f3a4f52
    mov [0xb8008], rax
    mov rax, 0x4f204f204f204f20
    mov [0xb8010], rax
    mov rax, 0x4f204f204f204f20
    mov [0xb8018], rax
    mov rax, 0x4f204f204f204f20
    mov [0xb8020], rax

    ; Halt the CPU if we return from kernel_main
.halt:
    hlt
    jmp .halt

; Implement this function to set up proper paging for long mode
setup_paging:
    ; Set up page tables here
    ret