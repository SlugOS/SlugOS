/* Set up stack */
.section .bss
.align 16
.global stack_bottom
stack_bottom:
    .skip 16384  /* Allocate 16 KiB for stack space */
stack_top:

/* _start is the entry point, which jumps into C main kernel (kernel_main) */
.section .text
.global _start
.type _start, @function
_start:
    /* Set up the stack */
    mov stack_top, %esp      /* Load the stack pointer with the top of the stack */
    
    /* Call the kernel main function */
    call kernel_main
