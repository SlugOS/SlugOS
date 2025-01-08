TARGET = bootx64.efi
SRCS := $(shell find kernel -name '*.c')

CFLAGS += -Ikernel/headers

include uefi/Makefile

run:
	uefi-run -b /usr/share/ovmf/x64/OVMF.4m.fd bootx64.efi