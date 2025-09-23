#pragma once

#include "cstdint"

// RISC-V CSR definitions
#define CSR_MSTATUS     0x300
#define CSR_MIE         0x304
#define CSR_MTVEC       0x305
#define CSR_MSCRATCH    0x340
#define CSR_MEPC        0x341
#define CSR_MCAUSE      0x342
#define CSR_MTVAL       0x343
#define CSR_MIP         0x344

// Machine interrupt enable bits
#define MIE_MSIE        (1 << 3)   // Machine software interrupt enable
#define MIE_MTIE        (1 << 7)   // Machine timer interrupt enable
#define MIE_MEIE        (1 << 11)  // Machine external interrupt enable

// Machine interrupt pending bits
#define MIP_MSIP        (1 << 3)   // Machine software interrupt pending
#define MIP_MTIP        (1 << 7)   // Machine timer interrupt pending
#define MIP_MEIP        (1 << 11)  // Machine external interrupt pending

// Machine status bits
#define MSTATUS_MIE     (1 << 3)   // Machine interrupt enable

// Interrupt cause codes
#define CAUSE_MACHINE_SOFTWARE_INT      3
#define CAUSE_MACHINE_TIMER_INT         7
#define CAUSE_MACHINE_EXTERNAL_INT      11
#define CAUSE_SUPERVISOR_SOFTWARE_INT   1
#define CAUSE_SUPERVISOR_TIMER_INT      5
#define CAUSE_SUPERVISOR_EXTERNAL_INT   9

// Interrupt statistics
struct InterruptStats {
    uint32_t machine_software_count;
    uint32_t machine_timer_count;
    uint32_t machine_external_count;
    uint32_t supervisor_software_count;
    uint32_t supervisor_timer_count;
    uint32_t supervisor_external_count;
    uint32_t unhandled_exception_count;
};

// Forward declarations for friend functions
extern "C" {
    void unhandled_exception_handler();
    void machine_software_interrupt_handler();
    void machine_timer_interrupt_handler();
    void machine_external_interrupt_handler();
    void supervisor_software_interrupt_handler();
    void supervisor_timer_interrupt_handler();
    void supervisor_external_interrupt_handler();
}

// Interrupt controller class
class InterruptController {
private:
    static InterruptStats stats;
    static bool initialized;
    
    // Friend functions for interrupt handlers
    friend void ::unhandled_exception_handler();
    friend void ::machine_software_interrupt_handler();
    friend void ::machine_timer_interrupt_handler();
    friend void ::machine_external_interrupt_handler();
    friend void ::supervisor_software_interrupt_handler();
    friend void ::supervisor_timer_interrupt_handler();
    friend void ::supervisor_external_interrupt_handler();
    
public:
    // Initialize interrupt system
    static void init();
    
    // Enable/disable global interrupts
    static void enable_global_interrupts();
    static void disable_global_interrupts();
    
    // Enable/disable specific interrupts
    static void enable_machine_timer_interrupt();
    static void disable_machine_timer_interrupt();
    static void enable_machine_software_interrupt();
    static void disable_machine_software_interrupt();
    static void enable_machine_external_interrupt();
    static void disable_machine_external_interrupt();
    
    // Trigger software interrupt
    static void trigger_software_interrupt();
    static void clear_software_interrupt();
    
    // Get interrupt statistics
    static const InterruptStats& get_stats() { return stats; }
    
    // Reset statistics
    static void reset_stats();
    
    // CSR access functions
    static uint32_t read_csr(uint32_t csr);
    static void write_csr(uint32_t csr, uint32_t value);
    static void set_csr_bits(uint32_t csr, uint32_t bits);
    static void clear_csr_bits(uint32_t csr, uint32_t bits);
};

