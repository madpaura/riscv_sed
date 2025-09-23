# RISC-V 32-bit Baremetal C++ Program

A complete baremetal C++ program for 32-bit RISC-V architecture that demonstrates standard C++ features including dynamic memory allocation, classes, and STL-like containers.

## Features

- **Baremetal Environment**: Runs without an operating system
- **C++ Standard Features**: 
  - Dynamic memory allocation (`new`/`delete`)
  - Classes with constructors/destructors
  - Template classes (SimpleMap)
  - Static member variables
- **Custom Standard Library**: Minimal implementations of `cstddef` and `cstdint`
- **Memory Management**: Simple heap allocator for dynamic allocation
- **Interrupt System**: Complete ISR vector table and interrupt handling
- **QEMU Compatible**: Designed to run on QEMU RISC-V virtual machine

## Project Structure

```
riscv/
├── Makefile              # Build configuration
├── linker.ld            # Memory layout and linker script
├── include/             # Header files
│   ├── cstddef          # Minimal cstddef implementation
│   ├── cstdint          # Minimal cstdint implementation
│   ├── memory.h         # Memory allocator interface
│   ├── interrupt.h      # Interrupt system interface
│   ├── simple_map.h     # Template map implementation
│   └── sample_class.h   # Sample C++ class
└── src/                 # Source files
    ├── start.S          # Assembly startup code with ISR vectors
    ├── memory.cpp       # Memory allocator implementation
    ├── interrupt.cpp    # Interrupt handler implementation
    ├── sample_class.cpp # Sample class implementation
    └── main.cpp         # Main program
```

## Prerequisites

- RISC-V GCC toolchain (`riscv64-unknown-elf-gcc`)
- QEMU RISC-V system emulator (`qemu-system-riscv64`)
- Make

### Installing Prerequisites (Ubuntu/Debian)

```bash
sudo apt install gcc-riscv64-unknown-elf qemu-system-riscv
```

**Note**: This project uses the 64-bit RISC-V toolchain to compile 32-bit RISC-V code by specifying the appropriate architecture flags (`rv32imac_zicsr`) and ABI (`ilp32`).

## Building

```bash
# Compile the program
make

# Clean build artifacts
make clean

# Show memory usage
make size
```

## Running

```bash
# Run in QEMU (32-bit RISC-V)
make qemu

# Debug with GDB
make debug
```

## Program Output

The program demonstrates various C++ features and outputs:

```
RISC-V Baremetal C++ Program
=============================

System initialized successfully!
Heap size: 134148184 bytes

=== Testing C++ Features ===
1. Testing memory allocation...
   Free memory: 134148184 bytes
   Allocated int with value: 42
   Allocated array: [0, 10, 20, 30, 40]
   Free memory after cleanup: 20 bytes

2. Testing DataProcessor class...
   Creating DataProcessor instance...
   Instance count: 1
   Testing map operations...
   Retrieved value for key 10: 1000
   Testing dynamic memory operations...
   Processed array: [21, 41, 61, 81, 101]
   Testing copy constructor...
   Instance count after copy: 2
   Instance count after copy destruction: 1
   Final free memory: [varies] bytes

=== All tests completed successfully! ===
Program finished. System will halt.
```

## Architecture Details

- **Target Architecture**: RV32IMAC with Zicsr extension
- **ABI**: ILP32 (32-bit integers, longs, and pointers)
- **Memory Model**: Medany (medium any code model)
- **Toolchain**: Uses riscv64-unknown-elf-gcc with 32-bit target flags

## Key Components

### Memory Allocator (`memory.h/cpp`)
- Simple bump allocator for dynamic memory
- Overrides global `new`/`delete` operators
- Provides heap statistics

### SimpleMap Template (`simple_map.h`)
- STL-like map implementation using linked list
- Supports insert, find, erase operations
- Template-based for type safety

### DataProcessor Class (`sample_class.h/cpp`)
- Demonstrates C++ class features
- Uses dynamic memory allocation
- Implements copy constructor and assignment operator
- Shows static member usage

### Startup Code (`start.S`)
- RISC-V assembly bootstrap with interrupt vector table
- Sets up stack and BSS
- Initializes interrupt vector table (mtvec)
- Calls global constructors/destructors
- Jumps to main function

### Interrupt System (`interrupt.h/cpp`)
- Complete interrupt vector table with 12 interrupt vectors
- Support for machine-level interrupts (software, timer, external)
- Support for supervisor-level interrupts
- Context-saving interrupt handlers in assembly
- C++ interrupt controller class with CSR access
- Interrupt statistics tracking
- Friend function access for C-style handlers

## Memory Layout

- **Text Section**: 0x80000000+ (executable code)
- **Data Section**: After text (initialized data)
- **BSS Section**: After data (uninitialized data)
- **Heap**: After BSS (dynamic allocation)
- **Stack**: Top of RAM (64KB, grows downward)

## Customization

- Modify `ARCH` in Makefile for different RISC-V extensions
- Adjust `STACK_SIZE` in linker.ld for different stack sizes
- Extend SimpleMap or create other STL-like containers
- Add more C++ standard library features as needed

## Limitations

- Simple bump allocator (no free list)
- No exception handling (disabled with `-fno-exceptions`)
- No RTTI (disabled with `-fno-rtti`)
- Minimal standard library implementation
- Single-threaded environment
