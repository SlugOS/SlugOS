#include <string.h>
#include <slug.h>
#include <stdarg.h>

char log[2048] = {0};

void add_to_log(const char *msg, ...) {
    va_list args;
    char temp[1024];
    
    va_start(args, msg);
    int value = va_arg(args, int);  // Assuming integer format
    snprintf(temp, sizeof(temp), msg, value);
    va_end(args);

    // Add the message to the log, ensuring we don't overflow
    if (strlen(log) + strlen(temp) + 2 < sizeof(log)) {  // +2 for \n and \0
        strcat(log, temp);
        strcat(log, "\n");
    }
}

void execute_command(const char *cmd) {
    if (strcmp(cmd, "HELP") == 0) {
        printk("\nAvailable commands:\n");
        printk("HELP - Show this help message\n");
        printk("CLEAR - Clear the screen\n");
        printk("LOG - Show the crash log\n");
    } else if (strcmp(cmd, "CLEAR") == 0) {
        // Add clear screen functionality here
        clear_screen();
    } else if (strcmp(cmd, "LOG") == 0) {
        // Print the system log
        printk("%s", log);
    } else if (strlen(cmd) > 0) {
        printk("\nUnknown command: %s\n", cmd);
    }
}