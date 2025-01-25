#include <drivers/vga.h>

void initialize() {
    // Setup VGA text mode so printk and other functions work
    terminal_initialize();
}