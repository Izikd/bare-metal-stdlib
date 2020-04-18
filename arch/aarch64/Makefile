# SPDX-License-Identifier: MIT

CROSS_COMPILE ?= aarch64-elf-

# Tools
AS := $(CROSS_COMPILE)gcc
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld

ARCH_AFLAGS :=

ARCH_CFLAGS := \
	-nostartfiles \
	-mstrict-align \

# Don't force evaluation, as $(CC) is still not available
ARCH_SYSROOT_LIBS = $(shell $(CC) -print-sysroot)/usr/lib
ARCH_LINKER_SCRIPT := $(ARCH_DIR)/linker_script.ld

ARCH_LDFLAGS = \
	-L $(ARCH_SYSROOT_LIBS) \
	-lc \
	-lgcc \
	-static \
	-T $(ARCH_LINKER_SCRIPT) \

ARCH_RUN = \
	qemu-system-aarch64 \
	-M virt \
	-cpu cortex-a57 \
	-m 4G \
	-smp 1 \
	-nographic \
	-serial mon:stdio \
	-nodefaults \
	-kernel $< \
