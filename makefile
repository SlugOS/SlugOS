include make.config

ALL: build boot-$(ARCH)

build: $(OBJS)
	$(CC) -T misc/$(ARCH)-linker.ld -o $(TARGET) $(LDFLAGS) $(OBJS) -lgcc

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET) img slugos.img

boot-x86_64:
	mkdir -p img/boot/grub
	cp misc/grub.cfg img/boot/grub/
	mv $(TARGET) img/boot/
	grub-mkrescue img/ -o slugos.img

run-x86_64:
	qemu-system-x86_64 -hda slugos.img $(QEMUFLAGS)

# Debug target: start kernel with GDB for debugging
debug-x86_64:
	qemu-system-x86_64 -s -S -hda slugos.img $(QEMUFLAGS)
