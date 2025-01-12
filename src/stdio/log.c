#include <slug.h>
#include <stdbool.h>

extern bool serial_status;

void klog(int level, char* data) {
    if (level == 0) {
        if (serial_status == false) {
            return;
        } else {
            print_serial("[DEBUG] ");
            print_serial(data);
        }
    } else if (level == 1) {
        terminal_setcolor(VGA_COLOR_GREEN);
        writestring("[OK] ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        writestring(data);
        return;
    } else if (level == 2) {
        terminal_setcolor(VGA_COLOR_BROWN);
        writestring("[WARN] ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        writestring(data);
        return;
    } else if (level == 3) {
        terminal_setcolor(VGA_COLOR_RED);
        writestring("[ERR] ");
        terminal_setcolor(VGA_COLOR_LIGHT_GREY);
        writestring(data);
        return;
    } else {
        return;
    }
}