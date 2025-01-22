ALL: build grub

include make.config

build: $(OBJS)
	$(CC) -T misc/linker.ld -o $(TARGET) $(LDFLAGS) $(OBJS) -lgcc

%.o: %.s
	$(AS) -g $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET) img slugos.img

grub:
	mkdir -p img/boot/grub
	cp misc/grub.cfg img/boot/grub/
	mv $(TARGET) img/boot/
	grub-mkrescue img/ -o slugos.img

run:
	qemu-system-i386 -hda slugos.img $(QEMUFLAGS)

# Debug target: start kernel in GDB for debugging
debug:
	qemu-system-i386 -s -S -hda slugos.img
