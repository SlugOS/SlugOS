#include <slug.h>

__attribute__((noreturn))
void exception_handler() {
    crash(0); // Call the red screen of death
}
