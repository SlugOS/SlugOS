#include <slug.h>
#include "exception.h"

__attribute__((noreturn))
void exception_handler(interrupt_frame_t *frame) {
    printf("Exception handler triggered by interrupt %d\n", frame->int_num);
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
    while(1);
}
