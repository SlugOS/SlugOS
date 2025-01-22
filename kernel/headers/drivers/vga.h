#ifndef VGA_H
#define VGA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

// This function sets all the variables needed for the below functions
void terminal_initialize(void);
// This can change the color of the text
void terminal_setcolor(uint8_t color);
// Get a color code for the text (fg) and the background (bg) which you can pass to terminal_setcolor(uint8_t color);
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);

#ifdef __cplusplus
}
#endif

#endif /* VGA_H */