#include <stdbool.h>
#include <drivers/io.h>
#include <stdint.h>
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

void key_handler(struct interrupt_frame *frame) {
    // Read from the data port
    uint8_t scancode = inb(0x60);
    
    // Determine if this is a key press or release
    bool is_release = (scancode & 0x80) != 0;
    uint8_t key = scancode & 0x7F;  // Remove the release bit
    
    // Basic US QWERTY scancode mapping
    const char *key_map[] = {
        "ERROR", "Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
        "-", "=", "Backspace", "Tab", "Q", "W", "E", "R", "T", "Y", "U",
        "I", "O", "P", "[", "]", "\n", "LCtrl", "A", "S", "D", "F",
        "G", "H", "J", "K", "L", ";", "'", "`", "LShift", "\\", "Z", "X",
        "C", "V", "B", "N", "M", ",", ".", "/", "RShift", "*", "LAlt",
        " "
    };
    
    // Only print on key press, not release
    if (!is_release && key < sizeof(key_map)/sizeof(key_map[0])) {
        printk("%s", key_map[key]);
    }
    
    // Acknowledge the interrupt to the master PIC
    outb(0x20, 0x20);
}