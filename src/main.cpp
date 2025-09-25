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
#include "simple_list.h"
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
    
    uart::puts("   Testing emplace_back - map.emplace_back(4, 40)...\n");
    map.emplace_back(4, 40);
    uart::puts("   Element emplaced successfully\n");
    
    uart::puts("   Map size after emplace_back: ");
    uart::print_number(map.size());
    uart::puts("\n");
    
    uart::puts("   Verifying emplaced value - map[4] = ");
    uart::print_number(map[4]);
    uart::puts("\n");
    
    uart::puts("   Map test completed successfully\n");
}

void test_list_functions() {
    uart::puts("=== Testing List Functions ===\n");
    
    uart::puts("1. Testing SimpleList (std::list alternative):\n");
    uart::puts("   Creating list...\n");
    
    SimpleList<int> list;
    uart::puts("   List created\n");
    
    uart::puts("   Testing push_back...\n");
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    uart::puts("   Elements added with push_back\n");
    
    uart::puts("   List size: ");
    uart::print_number(list.size());
    uart::puts("\n");
    
    uart::puts("   Testing front and back access:\n");
    uart::puts("   Front: ");
    uart::print_number(list.front());
    uart::puts(", Back: ");
    uart::print_number(list.back());
    uart::puts("\n");
    
    uart::puts("   Testing push_front...\n");
    list.push_front(5);
    uart::puts("   Element added with push_front\n");
    
    uart::puts("   New front: ");
    uart::print_number(list.front());
    uart::puts("\n");
    
    uart::puts("   Testing emplace_back...\n");
    list.emplace_back(40);
    uart::puts("   Element emplaced at back\n");
    
    uart::puts("   New back: ");
    uart::print_number(list.back());
    uart::puts("\n");
    
    uart::puts("   Testing emplace_front...\n");
    list.emplace_front(1);
    uart::puts("   Element emplaced at front\n");
    
    uart::puts("   New front: ");
    uart::print_number(list.front());
    uart::puts("\n");
    
    uart::puts("   Final list size: ");
    uart::print_number(list.size());
    uart::puts("\n");
    
    uart::puts("   Testing iterator - List contents: ");
    for (auto it = list.begin(); it != list.end(); ++it) {
        uart::print_number(*it);
        uart::puts(" ");
    }
    uart::puts("\n");
    
    uart::puts("   Testing pop operations...\n");
    list.pop_front();
    list.pop_back();
    uart::puts("   Popped front and back\n");
    
    uart::puts("   Size after pops: ");
    uart::print_number(list.size());
    uart::puts("\n");
    
    uart::puts("   List test completed successfully\n");
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
    uart::puts("\n");
    test_map_functions();
    uart::puts("\n");
    test_list_functions();
    
    uart::puts("\n=== All tests completed! ===\n");
    return 0;
}
