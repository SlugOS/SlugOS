# Porting

To port SlugOS to your device (assuming its not ported yet), you must provide the following.

- A cross compiler
- A multiboot1 compliant bootloader and boot.s file
- And the functions defined in the kernel/headers/drivers folder into your own arch/ folder with the source code and definitions for them to function properly.