#include <slug.h>

__attribute__((noreturn))
void exception_handler(int interrupt_number) {
    asm("cli");
    crash(interrupt_number); // Call the red screen of death
    asm("hlt");
}
