#include "memory.h"

// External symbols from linker script
extern "C" {
    extern uint8_t __heap_start;
    extern uint8_t __heap_end;
}

// Static member definitions
uint8_t* SimpleAllocator::heap_start = nullptr;
uint8_t* SimpleAllocator::heap_end = nullptr;
uint8_t* SimpleAllocator::heap_current = nullptr;

void SimpleAllocator::init() {
    heap_start = &__heap_start;
    heap_end = &__heap_end;
    heap_current = heap_start;
}

void* SimpleAllocator::allocate(size_t size) {
    // Align to 8-byte boundary
    size = (size + 7) & ~7;
    
    if (heap_current + size > heap_end) {
        return nullptr; // Out of memory
    }
    
    void* ptr = heap_current;
    heap_current += size;
    return ptr;
}

void SimpleAllocator::deallocate(void* ptr) {
    // Simple allocator - no deallocation for now
    // In a real implementation, you'd implement a free list
    (void)ptr;
}

size_t SimpleAllocator::get_free_memory() {
    return heap_end - heap_current;
}

// Global new/delete operators
void* operator new(size_t size) {
    return SimpleAllocator::allocate(size);
}

void* operator new[](size_t size) {
    return SimpleAllocator::allocate(size);
}

void operator delete(void* ptr) noexcept {
    SimpleAllocator::deallocate(ptr);
}

void operator delete[](void* ptr) noexcept {
    SimpleAllocator::deallocate(ptr);
}

void operator delete(void* ptr, size_t size) noexcept {
    (void)size;
    SimpleAllocator::deallocate(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size;
    SimpleAllocator::deallocate(ptr);
}
