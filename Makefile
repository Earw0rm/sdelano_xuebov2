BUILD_DIR = ./build
SOURCE_DIR = ./src
CPREFIX ?=riscv64-unknown-elf-
COPS =-fno-pie -no-pie -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -g -mcmodel=medany -mno-relax




$(BUILD_DIR)/%_c.o: $(SOURCE_DIR)/%.c
	mkdir -p $(@D)
	$(CPREFIX)gcc $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SOURCE_DIR)/%.S
	mkdir -p $(@D)
	$(CPREFIX)gcc $(COPS) -MMD -c $< -o $@

C_FILES = $(wildcard $(SOURCE_DIR)/*.c) $(wildcard $(SOURCE_DIR)/*/*.c) 
ASM_FILES = $(wildcard $(SOURCE_DIR)/*.S) $(wildcard $(SOURCE_DIR)/*/*.S)
OBJ_FILES = $(C_FILES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SOURCE_DIR)/%.S=$(BUILD_DIR)/%_s.o)
DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

kernel.elf: linker.ld $(OBJ_FILES)
	$(CPREFIX)ld -T linker.ld -o $(BUILD_DIR)/kernel.elf $(OBJ_FILES)
	$(CPREFIX)objdump -D -S $(BUILD_DIR)/kernel.elf > $(BUILD_DIR)/__debug.txt

all: kernel.elf

clean:
	rm -rf $(BUILD_DIR)/*


qemu_dump:
	qemu-system-riscv64 -s -S -machine virt -cpu rv64 \
	 -smp 4 -m 128M -nographic \
	 -serial mon:stdio -bios none -machine dumpdtb=$(BUILD_DIR)/qemu-virt.dtb 
	dtc -I dtb -O dts -o $(BUILD_DIR)/qemu-virt.dts $(BUILD_DIR)/qemu-virt.dtb

qemu: clean all
	qemu-system-riscv64 -s -S -machine virt -cpu rv64 \
	 -smp 4 -m 128M -nographic \
	 -serial mon:stdio -bios none -kernel $(BUILD_DIR)/kernel.elf 

debug: kernel.elf
	gdb-multiarch ./build/kernel.elf -ex "target extended-remote localhost:1234"