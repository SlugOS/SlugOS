#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <slug.h>

/* VGA text buffer memory location. */
#define VGA_MEMORY ((uint16_t*) 0xB8000)

/* Dimensions of the text-mode VGA terminal. */
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

/* Current terminal state. */
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

/* Helper function to pack VGA colors. */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

/* Helper function to encode a VGA character. */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

static void disable_cursor(void) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

/* Terminal initialization: clears the screen and sets initial state. */
void terminal_initialize(void) {
    disable_cursor(); // Disable the cursor to make it look nice
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }
}

/* Set the current terminal color. */
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

/* Set foreground and background colors. */
void terminal_setcolors(enum vga_color fg, enum vga_color bg) {
    terminal_color = vga_entry_color(fg, bg);
}

/* Print a character at a specific terminal position. */
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

/* Scroll the terminal when content exceeds the screen height. */
void terminal_scroll() {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
        }
    }
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_putentryat(' ', terminal_color, x, VGA_HEIGHT - 1);
    }
    terminal_row = VGA_HEIGHT - 1;
}

/* Output a character to the terminal. */
void putchar(char c) {
    if (c == '\n') {
        terminal_column = 0; // Reset column to the start of the line
        terminal_row++;      // Move to the next row
        if (terminal_row >= VGA_HEIGHT) {
            terminal_scroll();
        }
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;  // Wrap to the next line
            terminal_row++;
            if (terminal_row >= VGA_HEIGHT) {
                terminal_scroll();
            }
        }
    }
}

/* Print a null-terminated string to the terminal. */
void writestring(const char* data) {
    while (*data != '\0') {
        putchar(*data);
        data++;
    }
}
