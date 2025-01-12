#include <stdint.h>
#include <slug.h>

#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
#else
#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn))
void __stack_chk_fail(void) {
    writestring("SSP triggered\nHalting CPU\n");
    asm("cli; hlt"); // This will run cli to disable interrupts then hlt to halt the CPU (therefore the OS)
    while(1);
}