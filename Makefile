SRC_DIR := src
LIB_DIR := $(SRC_DIR)/lib
INCLUDE_DIRS := $(SRC_DIR)/include limine
BUILD_DIR := build
ISO_DIR := $(BUILD_DIR)/isodir

LIB_FILES := $(shell find -L $(LIB_DIR) -type f)
C_FILES := $(filter %.c, $(LIB_FILES))
NASM_FILES := $(filter %.asm, $(LIB_FILES))
OBJ_FILES := $(patsubst $(LIB_DIR)/%, $(BUILD_DIR)/%.o, $(LIB_FILES))
HEADER_DEPS := $(patsubst $(LIB_DIR)/%, $(BUILD_DIR)/%.d, $(LIB_FILES))

# Limine Variables
LIMINE_DIR := limine
LIMINE := $(LIMINE_DIR)/limine
LIMINE_HEADER := $(LIMINE_DIR)/limine.h
LIMINE_CONF := limine.conf

# OVMF Variables
OVMF_DIR := ovmf
OVMF := $(OVMF_DIR)/ovmf-code-x86_64.fd

# Output vars
OUTPUT := os
OUTPUT_BIN := $(BUILD_DIR)/$(OUTPUT).bin
OUTPUT_ISO := $(BUILD_DIR)/$(OUTPUT).iso
OUTPUT_HDD := $(BUILD_DIR)/$(OUTPUT).hdd

CC := clang

INCLUDE_FLAGS := $(addprefix -I , $(INCLUDE_DIRS))
CFLAGS := \
	$(INCLUDE_FLAGS) \
  -O2 \
	-target x86_64-unknown-none-elf \
  -Wall \
  -Wextra \
  -std=gnu11 \
  -ffreestanding \
  -fno-stack-protector \
  -fno-stack-check \
  -fno-lto \
  -fno-PIC \
  -ffunction-sections \
  -fdata-sections \
  -m64 \
  -march=x86-64 \
  -mabi=sysv \
  -mno-80387 \
  -mno-mmx \
  -mno-sse \
  -mno-sse2 \
  -mno-red-zone \
  -mcmodel=kernel \

NASMFLAGS := \
    -f elf64 \
    -Wall

LD := ld.lld
LDFLAGS := \
  -m elf_x86_64 \
  -nostdlib \
  -static \
  -z max-page-size=0x1000 \
  --gc-sections
LD_SCRIPT := limine_link.ld

.PHONY: all all-hdd run run-uefi run-hdd run-hdd-uefi clean

all: $(OUTPUT_ISO)

all-hdd: $(OUTPUT_HDD)

# Include header dependencies.
-include $(HEADER_DEPS)

$(BUILD_DIR)/%.c.o: $(LIB_DIR)/%.c
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.asm.o: $(LIB_DIR)/%.asm
	mkdir -p "$(dir $@)"
	nasm $(NASMFLAGS) $< -o $@

$(OUTPUT_BIN): $(LD_SCRIPT) $(OBJ_FILES)
	mkdir -p "$(dir $@)"
	$(LD) $(LDFLAGS) -T $(LD_SCRIPT) $(OBJ_FILES) -o $@

$(LIMINE):
	$(MAKE) -C $(LIMINE_DIR)

$(OUTPUT_ISO): $(LIMINE) $(LIMINE_CONF) $(OUTPUT_BIN)
	mkdir -p $(ISO_DIR)/boot
	cp -v $(OUTPUT_BIN) $(ISO_DIR)/boot/
	mkdir -p $(ISO_DIR)/boot/limine
	cp -v $(LIMINE_CONF) $(LIMINE_DIR)/limine-bios.sys \
				$(LIMINE_DIR)/limine-bios-cd.bin \
				$(ISO_DIR)/boot/limine/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		$(ISO_DIR) -o $(OUTPUT_ISO)
	./$(LIMINE) bios-install $(OUTPUT_ISO)

$(OUTPUT_HDD): $(LIMINE) $(LIMINE_CONF) $(OUTPUT_BIN)
	rm -f $(OUTPUT_HDD)
	dd if=/dev/zero bs=1M count=0 seek=64 of=$(OUTPUT_HDD)
	./$(LIMINE) bios-install $(OUTPUT_HDD)
	mformat -i $(OUTPUT_HDD)@@1M
	mmd -i $(OUTPUT_HDD)@@1M ::/boot ::/boot/limine
	mcopy -i $(OUTPUT_HDD)@@1M $(OUTPUT_BIN) ::/boot
	mcopy -i $(OUTPUT_HDD)@@1M $(LIMINE_CONF) $(LIMINE_DIR)/limine-bios.sys ::/boot/limine

QEMUFLAGS := -m 2G -serial stdio

run: $(OUTPUT_ISO)
	qemu-system-x86_64 \
		-M q35 \
		-cdrom $(OUTPUT_ISO) \
		-boot d \
		$(QEMUFLAGS)

run-hdd: $(OUTPUT_HDD)
	qemu-system-x86_64 \
		-M q35 \
		-hda $(OUTPUT_HDD) \
		$(QEMUFLAGS)

clean:
	rm -rf build
	$(MAKE) clean -C $(LIMINE_DIR)

