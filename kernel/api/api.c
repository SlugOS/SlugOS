#include <slug.h>

void api() {
    // This is just the standard API response for syscall not found
    printk("API called from userspace!\n");
}