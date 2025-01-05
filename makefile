ALL: grub

CFLAGS = -c -g -std=gnu99 -ffreestanding -Wall -Wextra -fstack-protector-all -Isrc/headers
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

# Find source files for C and Assembly
SRC_C = $(shell find src -name "*.c")
SRC_ASM = $(shell find src -name "*.s")
OBJ_C = $(patsubst src/%,obj/%, $(SRC_C:.c=.o))
OBJ_ASM = $(patsubst src/%,obj/%, $(SRC_ASM:.s=.o))

# Combine object files
OBJS = $(OBJ_C) $(OBJ_ASM)

# Final GRUB command to generate the ISO
grub: build
	mkdir -p iso/boot/grub
	cp grub/grub.cfg iso/boot/grub/grub.cfg
	cp slugos.elf iso/boot/slugos.elf
	# Create the ISO with GRUB configuration and custom boot
	grub-mkrescue -o slugos.iso iso
	rm -rf iso

# Build command that creates the ELF binary using custom linker script
build: $(OBJS)
	mkdir -p obj
	# Ensure the memory layout from linker.ld is applied
	i686-elf-gcc -T src/linker.ld -o slugos.elf $(LDFLAGS) $(OBJS)

# Compilation of C source files to object files
obj/%.o: src/%.c
	mkdir -p $(dir $@)
	i686-elf-gcc $(CFLAGS) -o $@ -c $<

# Compilation of Assembly source files to object files
obj/%.o: src/%.s
	mkdir -p $(dir $@)
	nasm -felf32 $< -o $@

# Clean the build artifacts
clean:
	rm -rf obj/* slugos.elf slugos.iso

# Run with QEMU for testing
run:
	qemu-system-i386 -cdrom slugos.iso

# Debug mode with GDB for kernel-level debugging
debug:
	qemu-system-i386 -cdrom slugos.iso -S -s & gdb -ex "target remote localhost:1234" -ex "file slugos.elf"
