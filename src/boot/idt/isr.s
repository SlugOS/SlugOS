.macro isr_err_stub name
.global \name
\name:
    call exception_handler
    iret
.endm

.macro isr_no_err_stub name
.global \name
\name:
    call exception_handler
    iret
.endm

isr_no_err_stub isr_stub_0
isr_no_err_stub isr_stub_1
isr_no_err_stub isr_stub_2
isr_no_err_stub isr_stub_3
isr_no_err_stub isr_stub_4
isr_no_err_stub isr_stub_5
isr_no_err_stub isr_stub_6
isr_no_err_stub isr_stub_7
isr_err_stub isr_stub_8
isr_err_stub isr_stub_9
isr_err_stub isr_stub_10
isr_err_stub isr_stub_11
isr_err_stub isr_stub_12
isr_err_stub isr_stub_13
isr_no_err_stub isr_stub_14
isr_no_err_stub isr_stub_15
isr_err_stub isr_stub_16
isr_err_stub isr_stub_17
isr_err_stub isr_stub_18
isr_err_stub isr_stub_19
isr_err_stub isr_stub_20
isr_err_stub isr_stub_21
isr_err_stub isr_stub_22
isr_err_stub isr_stub_23
isr_err_stub isr_stub_24
isr_err_stub isr_stub_25
isr_err_stub isr_stub_26
isr_err_stub isr_stub_27
isr_err_stub isr_stub_28
isr_err_stub isr_stub_29
isr_no_err_stub isr_stub_30
isr_no_err_stub isr_stub_31

.section .rodata
.align 4
.global isr_stub_table
isr_stub_table:
    .long isr_stub_0
    .long isr_stub_1
    .long isr_stub_2
    .long isr_stub_3
    .long isr_stub_4
    .long isr_stub_5
    .long isr_stub_6
    .long isr_stub_7
    .long isr_stub_8
    .long isr_stub_9
    .long isr_stub_10
    .long isr_stub_11
    .long isr_stub_12
    .long isr_stub_13
    .long isr_stub_14
    .long isr_stub_15
    .long isr_stub_16
    .long isr_stub_17
    .long isr_stub_18
    .long isr_stub_19
    .long isr_stub_20
    .long isr_stub_21
    .long isr_stub_22
    .long isr_stub_23
    .long isr_stub_24
    .long isr_stub_25
    .long isr_stub_26
    .long isr_stub_27
    .long isr_stub_28
    .long isr_stub_29
    .long isr_stub_30
    .long isr_stub_31
