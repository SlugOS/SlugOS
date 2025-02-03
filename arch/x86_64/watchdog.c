#include <stdint.h>
#include <slug.h>

#define WATCHDOG_TIMEOUT 500    // Timeout in milliseconds
extern volatile uint32_t watchdog_counter;

// Watchdog function to check if the system is stuck
void check_watchdog() {
    printk("Watchdog counter: %d\n", watchdog_counter);
    if (watchdog_counter >= WATCHDOG_TIMEOUT) {
        crash(33);
    }
}