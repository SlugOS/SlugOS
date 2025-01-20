#include <vga.h>
#include <slug.h>
#include <stdarg.h>

void logk(int level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    if (level == 0) {
        // This is prints debugging info
        #ifdef DEBUG
        terminal_setcolor(VGA_COLOR_LIGHT_BLUE);
        writestring("[DEBUG] ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        printk(fmt, args);
        #else
        // We are not in debug so do not print this
        return;
        #endif
    } else if (level == 1) {
        // This prints OK and is for telling the user that a part of the kernel initialized and nothing went wrong
        terminal_setcolor(VGA_COLOR_GREEN);
        writestring("[OK] ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        printk(fmt, args);
    } else if (level == 2) {
        // This prints WARN since it is for stuff like the serial port not working, which is not vital so will not cause any issues
        terminal_setcolor(VGA_COLOR_GREEN);
        writestring("[WARN] ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        printk(fmt, args);
    } else if (level == 3) {
        // This prints ERROR and tells the user that something went wrong, it will not function
        terminal_setcolor(VGA_COLOR_GREEN);
        writestring("[ERR] ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        printk(fmt, args);
    } else if (level == 4) {
        // This prints CRIT ERR and halts the CPU
        terminal_setcolor(VGA_COLOR_GREEN);
        writestring("[OK] ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        printk(fmt, args);
        asm("cli; hlt");
    }

    va_end(args);
}