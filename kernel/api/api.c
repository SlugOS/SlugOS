#include <slug.h>

void api(int eax, int ebx) {
    if (eax == 1) {
        // This is the crash handler so do crash(ebx);
        crash(ebx);
    } else {
        // This is just the standard API response for syscall not found
        printk("API called from userspace!\n");
    }
}