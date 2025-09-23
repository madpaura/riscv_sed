# RISC-V Baremetal C++ Makefile

# Toolchain configuration
CROSS_COMPILE = riscv64-unknown-elf-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

# Target configuration
TARGET = riscv-program
ARCH = rv32imac_zicsr
ABI = ilp32

# Directories
SRC_DIRS = src src/drivers src/kernel src/lib
BUILD_DIR = build
INCLUDE_DIRS = include include/drivers include/kernel include/lib

# Source files (recursively find in all source directories)
CPP_SOURCES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
ASM_SOURCES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.S))

# Generate object file paths, preserving directory structure
CPP_OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))
ASM_OBJECTS = $(patsubst %.S,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
OBJECTS = $(CPP_OBJECTS) $(ASM_OBJECTS)

# Create list of all subdirectories that need to be created in build
BUILD_SUBDIRS = $(sort $(dir $(OBJECTS)))

# Compiler flags
CPPFLAGS = $(addprefix -I,$(INCLUDE_DIRS)) -march=$(ARCH) -mabi=$(ABI) -mcmodel=medany
CXXFLAGS = -std=c++17 -O2 -g -Wall -Wextra -fno-exceptions -fno-rtti -fno-threadsafe-statics \
           -ffunction-sections -fdata-sections -nostdlib -nostartfiles -ffreestanding
ASFLAGS = -march=$(ARCH) -mabi=$(ABI)
LDFLAGS = -nostdlib -nostartfiles -T linker.ld -Wl,--gc-sections -Wl,-m,elf32lriscv

# Default target
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).dump

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Create all necessary subdirectories
$(BUILD_SUBDIRS): | $(BUILD_DIR)
	mkdir -p $@

# Compile C++ source files (pattern rule for any source directory)
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_SUBDIRS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Compile assembly source files (pattern rule for any source directory)
$(BUILD_DIR)/%.o: %.S | $(BUILD_SUBDIRS)
	$(AS) $(ASFLAGS) -c $< -o $@

# Link the final executable
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

# Create binary file
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Create disassembly dump
$(BUILD_DIR)/$(TARGET).dump: $(BUILD_DIR)/$(TARGET).elf
	$(OBJDUMP) -D $< > $@

# Run in QEMU
qemu: $(BUILD_DIR)/$(TARGET).elf
	qemu-system-riscv32 -machine virt -cpu rv32 -smp 1 -m 128M -nographic \
		-bios none -kernel $(BUILD_DIR)/$(TARGET).elf

# Debug with QEMU and GDB
debug: $(BUILD_DIR)/$(TARGET).elf
	qemu-system-riscv32 -machine virt -cpu rv32 -smp 1 -m 128M -nographic \
		-bios none -kernel $(BUILD_DIR)/$(TARGET).elf -s -S &
	$(CROSS_COMPILE)gdb $(BUILD_DIR)/$(TARGET).elf -ex "target remote :1234"

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Show memory usage
size: $(BUILD_DIR)/$(TARGET).elf
	$(CROSS_COMPILE)size $<

# Show project structure
structure:
	@echo "Source directories: $(SRC_DIRS)"
	@echo "Include directories: $(INCLUDE_DIRS)"
	@echo "C++ sources found: $(CPP_SOURCES)"
	@echo "ASM sources found: $(ASM_SOURCES)"
	@echo "Build subdirectories: $(BUILD_SUBDIRS)"

# Phony targets
.PHONY: all clean qemu debug size structure

# Print variables for debugging
print-%:
	@echo $* = $($*)
