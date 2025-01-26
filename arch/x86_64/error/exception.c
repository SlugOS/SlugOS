#include <slug.h>

__attribute__((noreturn))
void exception_handler(int interrupt_number) {
    crash(interrupt_number);
}