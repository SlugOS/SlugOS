#include <string.h>
#include <slug.h>

char log[2048] = {0};

void add_to_log(const char *msg) {
    // Add the message to the log
    strcat(log, msg);
    // Then add in a newline character
    strcat(log, "\n");
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
        // Print the log
        printk("%s", log);
    } else if (strlen(cmd) > 0) {
        printk("\nUnknown command: %s\n", cmd);
    }
}