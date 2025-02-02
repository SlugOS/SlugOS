#include <stdbool.h>
#include <drivers/io.h>
#include <stdint.h>
#include <string.h>
#include <slug.h>

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

#define BUFFER_SIZE 256
#define ENTER_KEY 0x1C

static char cmd_buffer[BUFFER_SIZE];
static int buffer_pos = 0;

extern bool debug;

void execute_command(const char *cmd, struct interrupt_frame *frame);

void key_handler(struct interrupt_frame *frame) {
    uint8_t scancode = inb(0x60);
    bool is_release = (scancode & 0x80) != 0;
    uint8_t key = scancode & 0x7F;

    const char *key_map[] = {
        "ERROR", "", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
        "-", "=", "\b", "\t", "Q", "W", "E", "R", "T", "Y", "U",
        "I", "O", "P", "[", "]", "\n", "", "A", "S", "D", "F",
        "G", "H", "J", "K", "L", ";", "'", "`", "", "\\", "Z", "X",
        "C", "V", "B", "N", "M", ",", ".", "/", "", "*", "",
        " "
    };


    if (!is_release && key < sizeof(key_map)/sizeof(key_map[0])) {
        if (debug) {
            if (key == ENTER_KEY) {
                cmd_buffer[buffer_pos] = '\0';
                printk("\n");
                execute_command(cmd_buffer, frame);
                buffer_pos = 0;
                memset(cmd_buffer, 0, BUFFER_SIZE);
                printk("> ");
            } else if (strcmp(key_map[key], "\b") == 0 && buffer_pos > 0) {
                buffer_pos--;
                printk("\b \b");
            } else if (buffer_pos < BUFFER_SIZE - 1 && strlen(key_map[key]) == 1) {
                cmd_buffer[buffer_pos++] = key_map[key][0];
                printk("%s", key_map[key]);
            }
        } else {
            printk("%s", key_map[key]);
        }
    }

    outb(0x20, 0x20);
}