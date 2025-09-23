#pragma once

#include "cstddef"
#include "cstdint"

// Simple heap allocator for baremetal environment
class SimpleAllocator {
private:
    static uint8_t* heap_start;
    static uint8_t* heap_end;
    static uint8_t* heap_current;
    
public:
    static void init();
    static void* allocate(size_t size);
    static void deallocate(void* ptr);
    static size_t get_free_memory();
};

// Override global new/delete operators
void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete(void* ptr, size_t size) noexcept;
void operator delete[](void* ptr, size_t size) noexcept;
