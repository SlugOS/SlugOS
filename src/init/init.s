.global _init
.type _init, @function
_init:
    # Setup basic segmentation using GDT
    call gdt_init
    # Setup the IDT
    call idt_init
    # Setup the needed variables and such for the rest of the VGA text mode functions
    call terminal_initialize
    ret