# Makefile for assembling, compiling, and linking the kernel

# Toolchain
AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
QEMU = qemu-system-arm

# File paths
SRC_DIR = src
OBJ_DIR = obj
OUT_DIR = out
KERNEL_NAME = kernel
LINKER_SCRIPT = link.ld

# Flags
CFLAGS = -Iheaders -Wall -Wextra -Werror -nostdlib -nostartfiles -ffreestanding -std=gnu99 -c
LDFLAGS = -T $(LINKER_SCRIPT) -o $(OUT_DIR)/$(KERNEL_NAME)

# Automatically find all source files
ASMS = $(wildcard $(SRC_DIR)/*.s)
CS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(ASMS:$(SRC_DIR)/%.s=$(OBJ_DIR)/%.o) $(CS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target
all: $(OUT_DIR)/$(KERNEL_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(OBJ_DIR)
	$(AS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(OUT_DIR)/$(KERNEL_NAME): $(OBJS)
	@mkdir -p $(OUT_DIR)
	$(LD) $(LDFLAGS) $(OBJS)

# Boot the kernel with QEMU
run: $(OUT_DIR)/$(KERNEL_NAME)
	$(QEMU) -machine integratorcp -m 128 -kernel $(OUT_DIR)/$(KERNEL_NAME)

clean:
	rm -rf $(OBJ_DIR) $(OUT_DIR)

.PHONY: all run clean
