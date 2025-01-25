include make.config

ALL: build boot-$(ARCH)

build: $(OBJS)
	$(CC) -T misc/linker.ld -o $(TARGET) $(LDFLAGS) $(OBJS) -lgcc

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET) img slugos.img

boot-i686:
	mkdir -p img/boot/grub
	cp misc/grub.cfg img/boot/grub/
	mv $(TARGET) img/boot/
	grub-mkrescue img/ -o slugos.img

run-i686:
	qemu-system-i386 -hda slugos.img $(QEMUFLAGS)

# Debug target: start kernel in GDB for debugging
debug-i686:
	qemu-system-i386 -s -S -hda slugos.img $(QEMUFLAGS) -serial tcp::1234,server
