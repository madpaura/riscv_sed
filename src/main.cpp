#include "memory.h"

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <cstddef>
#include "sample_class.h"

// Workaround for max_align_t issue in cross-compilation
#ifndef __DEFINED_max_align_t
typedef long double max_align_t;
#define __DEFINED_max_align_t
#endif

#include <map>
#include <vector>
#include "simple_map.h"
#include "uart.h"
#include <interrupt.h>
#include <interrupt.h>

void test_stdlib_functions() {
    uart::puts("=== Testing Standard Library Functions ===\n");
    
    // Test malloc/free
    uart::puts("1. Testing malloc/free:\n");
    void* ptr = malloc(100);
    if (ptr) {
        uart::puts("   malloc(100) successful\n");
        free(ptr);
        uart::puts("   free() successful\n");
    }
    
    // Test string functions
    uart::puts("2. Testing string functions:\n");
    char str1[50] = "Hello";
    char str2[50] = " World";
    char combined[100];
    
    strcpy(combined, str1);
    strcat(combined, str2);
    uart::puts("   strcpy + strcat result: ");
    uart::puts(combined);
    uart::puts("\n   Length: ");
    // uart::print_number(strlen(combined));
    uart::puts("\n");
 
    // Test memory functions
    uart::puts("3. Testing memory functions:\n");
    int numbers[5] = {1, 2, 3, 4, 5};
    int copy[5];
    memcpy(copy, numbers, sizeof(numbers));
    uart::puts("   memcpy result: [");
    for (int i = 0; i < 5; i++) {
        uart::print_number(copy[i]);
        if (i < 4) uart::puts(", ");
    }
    uart::puts("]\n");
    
    // Test math functions
    uart::puts("4. Testing math functions:\n");
    double x = 4.0;
    uart::puts("   sqrt(4.0) = ");
    uart::print_number((int)sqrt(x));
    uart::puts("\n   abs(-42) = ");
    uart::print_number(abs(-42));
    uart::puts("\n");
}

class SampleClass {
public:
    SampleClass() {
        uart::puts("SampleClass constructor called\n");
    }
    
    void print() {
        uart::puts("SampleClass instance created\n");
    }
};


void test_class_functions(){
    uart::puts("=== Testing Class Functions ===\n");
    
    // Test class functions
    uart::puts("1. Testing class functions:\n");
    SampleClass obj;
    obj.print();

    SampleClass* obj2 = new SampleClass();
    obj2->print();
    delete obj2;
}

void test_map_functions(){
    uart::puts("=== Testing Map Functions ===\n");
    
    uart::puts("1. Testing SimpleMap (std::map alternative):\n");
    uart::puts("   Creating map...\n");
    
    SimpleMap<int, int> map;
    uart::puts("   Map created\n");
    
    uart::puts("   Using operator[] - map[1] = 1...\n");
    map[1] = 1;
    uart::puts("   Element inserted using operator[]\n");
    
    uart::puts("   Map size: ");
    uart::print_number(map.size());
    uart::puts("\n");
    
    uart::puts("   Using operator[] - map[2] = 2...\n");
    map[2] = 2;
    uart::puts("   Second element inserted using operator[]\n");
    
    uart::puts("   Map size: ");
    uart::print_number(map.size());
    uart::puts("\n");
    
    uart::puts("   Using operator[] - map[3] = 3...\n");
    map[3] = 3;
    uart::puts("   Third element inserted using operator[]\n");
    
    uart::puts("   Map size: ");
    uart::print_number(map.size());
    uart::puts("\n");
    
    uart::puts("   Testing retrieval - map[2] = ");
    uart::print_number(map[2]);
    uart::puts("\n");
    
    uart::puts("   Testing modification - map[2] = 20...\n");
    map[2] = 20;
    uart::puts("   Value modified using operator[]\n");
    
    uart::puts("   Verifying modification - map[2] = ");
    uart::print_number(map[2]);
    uart::puts("\n");
    
    uart::puts("   Map test completed successfully\n");
}

void test_math_functions() {
    uart::puts("=== Testing Math Functions ===\n");
    
    // Test basic math functions
    uart::puts("1. Testing basic math:\n");
    uart::puts("   abs(-42) = ");
    uart::print_number(abs(-42));
    uart::puts("\n");
    
    uart::puts("   sqrt(16) = ");
    uart::print_number((int)sqrt(16.0));
    uart::puts("\n");
    
    uart::puts("   pow(2, 3) = ");
    uart::print_number((int)pow(2.0, 3.0));
    uart::puts("\n");
    
    // Test random numbers
    uart::puts("2. Testing random numbers:\n");
    srand(42);  // Seed for reproducible results
    uart::puts("   Random numbers: ");
    for (int i = 0; i < 5; i++) {
        uart::print_number(rand() % 100);
        uart::puts(" ");
    }
    uart::puts("\n");
}

extern "C" int main() {
    uart::puts("RISC-V C++ Program with Standard Library\n");
    uart::puts("========================================\n\n");

    SimpleAllocator::init();
    InterruptController::init();
    InterruptController::enable_global_interrupts();

    test_stdlib_functions();
    uart::puts("\n");
    test_class_functions();
    // test_math_functions();
    // uart::puts("\n");
    test_map_functions();
    
    uart::puts("\n=== All tests completed! ===\n");
    return 0;
}
