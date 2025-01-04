ENTRY (entry)

SECTIONS
{
    . = 0x10000;
    .text : { *(.text*) *(.rodata*) *(.data*) *(.bss*) *(COMMON) }
     _EOI = .;
}
