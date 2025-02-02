#include <drivers/io.h>
#include <string.h>
#include <slug.h>
#include <stdarg.h>

struct interrupt_frame {
    // Pushed by our ISR stub
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    // Pushed by CPU
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

char log[4096] = {0};

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

void reboot() {
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    asm("hlt");
}

void execute_command(const char *cmd, struct interrupt_frame *frame) {
    if (strcmp(cmd, "HELP") == 0) {
        printk("\nAvailable commands:\n");
        printk("HELP - Show this help message\n");
        printk("CLEAR - Clear the screen\n");
        printk("LOG - Show the crash log\n");
        printk("TRACE - Show a stack trace for the last 10 functions\n");
        printk("REBOOT - Reboot the system\n");
        printk("REG - Show the register values\n");
    } else if (strcmp(cmd, "CLEAR") == 0) {
        // Add clear screen functionality here
        clear_screen();
    } else if (strcmp(cmd, "LOG") == 0) {
        // Print the system log
        printk("%s", log);
    } else if (strcmp(cmd, "TRACE") == 0) {
        // Show a stack trace of the last 10 functions
        TraceStackTrace(10);
    } else if (strcmp(cmd, "REBOOT") == 0) {
        reboot();
    } else if (strcmp(cmd, "REG") == 0) {
        printk("Interrupt Frame Dump:\n");

        printk(" R15 = 0x%x\n", frame->r15);
        printk(" R14 = 0x%x\n", frame->r14);
        printk(" R13 = 0x%x\n", frame->r13);
        printk(" R12 = 0x%x\n", frame->r12);
        printk(" R11 = 0x%x\n", frame->r11);
        printk(" R10 = 0x%x\n", frame->r10);
        printk(" R9 = 0x%x\n", frame->r9);
        printk(" R8 = 0x%x\n", frame->r8);
        printk(" RBP = 0x%x\n", frame->rbp);
        printk(" RDI = 0x%x\n", frame->rdi);
        printk(" RSI = 0x%x\n", frame->rsi);
        printk(" RDX = 0x%x\n", frame->rdx);
        printk(" RCX = 0x%x\n", frame->rcx);
        printk(" RBX = 0x%x\n", frame->rbx);
        printk(" RAX = 0x%x\n", frame->rax);

        printk(" RIP = 0x%x\n", frame->rip);
        printk(" CS = 0x%x\n", frame->cs);
        printk(" RFLAGS = 0x%x\n", frame->rflags);
        printk(" RSP = 0x%x\n", frame->rsp);
        printk(" SS = 0x%x\n", frame->ss);
    } else if (strlen(cmd) > 0) {
        printk("\nUnknown command: %s\n", cmd);
    }
}