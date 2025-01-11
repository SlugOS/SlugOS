# Assembler and compiler
AS = i686-elf-as
CC = i686-elf-gcc

# C flags
CFLAGS = -g -c -std=gnu99 -ffreestanding -Wall -Wextra -Werror -fstack-protector-all -Isrc/headers

# Linker flags
LDFLAGS = -ffreestanding -O2 -nostdlib

# Get all .s and .c files in src/
SRCS = $(shell find src -name "*.s" -o -name "*.c")

# The kernel binary we compile
TARGET=slug.elf

# Object files generated from the .s and .c files
OBJS = $(SRCS:.s=.o)
OBJS := $(OBJS:.c=.o)

build: $(OBJS)
	$(CC) -T misc/linker.ld -o $(TARGET) $(LDFLAGS) $(OBJS) -lgcc

%.o: %.s
	$(AS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET) img

grub:
	mkdir -p img/boot/grub
	cp misc/grub.cfg img/boot/grub/
	mv $(TARGET) img/boot/
	grub-mkrescue img/ -o slugos.img

run: grub
	qemu-system-x86_64 -hda slugos.img

# Debug target: start kernel in GDB for debugging
debug: grub
	qemu-system-x86_64 -s -S -hda slugos.img