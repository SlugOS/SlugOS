; All the things we need to init
extern terminal_initialize
extern gdt_init
extern idt_init

; make the _init function able to be called from other files like boot.s
global _init

_init:
    call terminal_initialize
    call gdt_init
    call idt_init
    ret