ALL: grub

CFLAGS = -c -g -std=gnu99 -ffreestanding -Wall -Wextra -fstack-protector-all -Isrc/headers
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

SRC_C = $(shell find src -name "*.c")
SRC_ASM = $(shell find src -name "*.s")
OBJ_C = $(patsubst src/%,obj/%, $(SRC_C:.c=.o))
OBJ_ASM = $(patsubst src/%,obj/%, $(SRC_ASM:.s=.o))

OBJS = $(OBJ_C) $(OBJ_ASM)

grub: build
	mkdir -p iso/boot/grub
	cp grub/grub.cfg iso/boot/grub/grub.cfg
	cp slugos.elf iso/boot/slugos.elf
	grub-mkrescue -o slugos.iso iso
	rm -rf iso

build: $(OBJS)
	mkdir -p obj
	i686-elf-gcc -T src/linker.ld -o slugos.elf $(LDFLAGS) $(OBJS)

obj/%.o: src/%.c
	mkdir -p $(dir $@)
	i686-elf-gcc $(CFLAGS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(dir $@)
	nasm -felf32 $< -o $@

clean:
	rm -rf obj/* slugos.elf slugos.iso

run:
	qemu-system-i386 -cdrom slugos.iso

debug:
	qemu-system-i386 -cdrom slugos.iso -S -s & gdb -ex "target remote localhost:1234" -ex "file slugos.elf"
