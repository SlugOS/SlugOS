global timer_stub
extern IRQ0_handler

timer_stub:
    call IRQ0_handler
    iretq