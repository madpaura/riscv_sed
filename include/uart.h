#include <cstdint>

// Simple UART functions for output
namespace uart {
    constexpr uint64_t UART_BASE = 0x10000000;
    constexpr uint64_t UART_THR = UART_BASE + 0x00;
    constexpr uint64_t UART_LSR = UART_BASE + 0x05;
    
    static void putchar(char c) {
        while ((*(volatile uint8_t*)UART_LSR & 0x20) == 0) {}
        *(volatile uint8_t*)UART_THR = c;
    }
    
    static void puts(const char* str) {
        while (*str) {
            putchar(*str++);
        }
    }
    
    static void print_number(uint32_t num) {
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
        
        while (i > 0) {
            putchar(buffer[--i]);
        }
    }
}
