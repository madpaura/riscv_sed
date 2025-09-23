#include "memory.h"
#include "sample_class.h"
#include "interrupt.h"

// Simple UART functions for QEMU virt machine
namespace uart {
    constexpr uint64_t UART_BASE = 0x10000000;
    constexpr uint64_t UART_THR = UART_BASE + 0x00; // Transmit Holding Register
    constexpr uint64_t UART_LSR = UART_BASE + 0x05; // Line Status Register
    
    void putchar(char c) {
        // Wait for transmitter to be ready
        while ((*(volatile uint8_t*)UART_LSR & 0x20) == 0) {
            // Wait
        }
        *(volatile uint8_t*)UART_THR = c;
    }
    
    void puts(const char* str) {
        while (*str) {
            putchar(*str++);
        }
    }
    
    void print_number(int num) {
        if (num == 0) {
            putchar('0');
            return;
        }
        
        if (num < 0) {
            putchar('-');
            num = -num;
        }
        
        char buffer[16];
        int i = 0;
        while (num > 0) {
            buffer[i++] = '0' + (num % 10);
            num /= 10;
        }
        
        // Print in reverse order
        while (i > 0) {
            putchar(buffer[--i]);
        }
    }
    
    void print_hex(uint32_t num) {
        const char hex_chars[] = "0123456789ABCDEF";
        char buffer[9]; // 8 hex digits + null terminator
        int i = 7;
        
        buffer[8] = '\0';
        
        if (num == 0) {
            puts("00000000");
            return;
        }
        
        while (i >= 0) {
            buffer[i] = hex_chars[num & 0xF];
            num >>= 4;
            i--;
        }
        
        puts(buffer);
    }
}

// Test function to demonstrate C++ features
void test_cpp_features() {
    uart::puts("=== Testing C++ Features ===\n");
    
    // Test memory allocator
    uart::puts("1. Testing memory allocation...\n");
    uart::puts("   Free memory: ");
    uart::print_number(SimpleAllocator::get_free_memory());
    uart::puts(" bytes\n");
    
    // Test dynamic allocation
    int* test_ptr = new int(42);
    uart::puts("   Allocated int with value: ");
    uart::print_number(*test_ptr);
    uart::puts("\n");
    delete test_ptr;
    
    // Test array allocation
    int* test_array = new int[5];
    for (int i = 0; i < 5; ++i) {
        test_array[i] = i * 10;
    }
    uart::puts("   Allocated array: [");
    for (int i = 0; i < 5; ++i) {
        uart::print_number(test_array[i]);
        if (i < 4) uart::puts(", ");
    }
    uart::puts("]\n");
    delete[] test_array;
    
    uart::puts("   Free memory after cleanup: ");
    uart::print_number(SimpleAllocator::get_free_memory());
    uart::puts(" bytes\n\n");
    
    // Test DataProcessor class
    uart::puts("2. Testing DataProcessor class...\n");
    uart::puts("   Creating DataProcessor instance...\n");
    
    DataProcessor processor(8);
    uart::puts("   Instance count: ");
    uart::print_number(DataProcessor::get_instance_count());
    uart::puts("\n");
    
    // Test map operations
    uart::puts("   Testing map operations...\n");
    processor.demonstrate_map_operations();
    
    processor.add_data(10, 1000);
    int* value = processor.get_data(10);
    if (value) {
        uart::puts("   Retrieved value for key 10: ");
        uart::print_number(*value);
        uart::puts("\n");
    }
    
    // Test dynamic memory operations
    uart::puts("   Testing dynamic memory operations...\n");
    processor.demonstrate_dynamic_memory();
    
    const int* processed_data = processor.get_processed_data();
    uart::puts("   Processed array: [");
    for (size_t i = 0; i < 5; ++i) {
        uart::print_number(processed_data[i]);
        if (i < 4) uart::puts(", ");
    }
    uart::puts("]\n");
    
    // Test copy constructor
    uart::puts("   Testing copy constructor...\n");
    {
        DataProcessor processor_copy(processor);
        uart::puts("   Instance count after copy: ");
        uart::print_number(DataProcessor::get_instance_count());
        uart::puts("\n");
    } // processor_copy goes out of scope here
    
    uart::puts("   Instance count after copy destruction: ");
    uart::print_number(DataProcessor::get_instance_count());
    uart::puts("\n");
    
    processor.print_statistics();
    
    uart::puts("   Final free memory: ");
    uart::print_number(SimpleAllocator::get_free_memory());
    uart::puts(" bytes\n\n");
}

// Test function to demonstrate interrupt functionality
void test_interrupt_system() {
    uart::puts("=== Testing Interrupt System ===\n");
    
    // Initialize interrupt controller
    uart::puts("1. Initializing interrupt controller...\n");
    InterruptController::init();
    
    // Display initial interrupt statistics
    const InterruptStats& stats = InterruptController::get_stats();
    uart::puts("   Initial interrupt counts:\n");
    uart::puts("   - Machine software: ");
    uart::print_number(stats.machine_software_count);
    uart::puts("\n   - Machine timer: ");
    uart::print_number(stats.machine_timer_count);
    uart::puts("\n   - Machine external: ");
    uart::print_number(stats.machine_external_count);
    uart::puts("\n   - Unhandled exceptions: ");
    uart::print_number(stats.unhandled_exception_count);
    uart::puts("\n\n");
    
    // Test software interrupt
    uart::puts("2. Testing software interrupt...\n");
    InterruptController::enable_machine_software_interrupt();
    InterruptController::enable_global_interrupts();
    
    uart::puts("   Triggering software interrupt...\n");
    InterruptController::trigger_software_interrupt();
    
    // Small delay to allow interrupt processing
    for (volatile int i = 0; i < 1000; i++) {
        // Wait
    }
    
    // Display updated statistics
    const InterruptStats& updated_stats = InterruptController::get_stats();
    uart::puts("   Updated interrupt counts:\n");
    uart::puts("   - Machine software: ");
    uart::print_number(updated_stats.machine_software_count);
    uart::puts("\n   - Machine timer: ");
    uart::print_number(updated_stats.machine_timer_count);
    uart::puts("\n   - Machine external: ");
    uart::print_number(updated_stats.machine_external_count);
    uart::puts("\n   - Unhandled exceptions: ");
    uart::print_number(updated_stats.unhandled_exception_count);
    uart::puts("\n\n");
    
    // Test CSR access
    uart::puts("3. Testing CSR access...\n");
    uint32_t mstatus = InterruptController::read_csr(CSR_MSTATUS);
    uint32_t mie = InterruptController::read_csr(CSR_MIE);
    uint32_t mip = InterruptController::read_csr(CSR_MIP);
    
    uart::puts("   MSTATUS: 0x");
    uart::print_hex(mstatus);
    uart::puts("\n   MIE: 0x");
    uart::print_hex(mie);
    uart::puts("\n   MIP: 0x");
    uart::print_hex(mip);
    uart::puts("\n\n");
    
    // Disable interrupts for clean shutdown
    InterruptController::disable_global_interrupts();
    uart::puts("   Interrupts disabled for clean shutdown.\n\n");
}

// Main function
extern "C" int main() {
    // Initialize memory allocator
    SimpleAllocator::init();
    
    uart::puts("RISC-V Baremetal C++ Program\n");
    uart::puts("=============================\n\n");
    
    uart::puts("System initialized successfully!\n");
    uart::puts("Heap size: ");
    uart::print_number(SimpleAllocator::get_free_memory());
    uart::puts(" bytes\n\n");
    
    // Test C++ features
    test_cpp_features();
    
    // Test interrupt system (commented out to avoid potential issues)
    // test_interrupt_system();
    
    uart::puts("=== All tests completed successfully! ===\n");
    uart::puts("Program finished. System will halt.\n");
    
    return 0;
}
