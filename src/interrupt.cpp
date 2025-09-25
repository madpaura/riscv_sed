#include "interrupt.h"

// Static member definitions
InterruptStats InterruptController::stats = {0, 0, 0, 0, 0, 0, 0};
bool InterruptController::initialized = false;

// CSR access inline assembly functions
uint32_t InterruptController::read_csr(uint32_t csr) {
    uint32_t value;
    switch (csr) {
        case CSR_MSTATUS:
            asm volatile ("csrr %0, mstatus" : "=r" (value));
            break;
        case CSR_MIE:
            asm volatile ("csrr %0, mie" : "=r" (value));
            break;
        case CSR_MIP:
            asm volatile ("csrr %0, mip" : "=r" (value));
            break;
        case CSR_MCAUSE:
            asm volatile ("csrr %0, mcause" : "=r" (value));
            break;
        case CSR_MEPC:
            asm volatile ("csrr %0, mepc" : "=r" (value));
            break;
        case CSR_MTVAL:
            asm volatile ("csrr %0, mtval" : "=r" (value));
            break;
        default:
            value = 0;
            break;
    }
    return value;
}

void InterruptController::write_csr(uint32_t csr, uint32_t value) {
    switch (csr) {
        case CSR_MSTATUS:
            asm volatile ("csrw mstatus, %0" : : "r" (value));
            break;
        case CSR_MIE:
            asm volatile ("csrw mie, %0" : : "r" (value));
            break;
        case CSR_MIP:
            asm volatile ("csrw mip, %0" : : "r" (value));
            break;
        case CSR_MEPC:
            asm volatile ("csrw mepc, %0" : : "r" (value));
            break;
        default:
            break;
    }
}

void InterruptController::set_csr_bits(uint32_t csr, uint32_t bits) {
    switch (csr) {
        case CSR_MSTATUS:
            asm volatile ("csrs mstatus, %0" : : "r" (bits));
            break;
        case CSR_MIE:
            asm volatile ("csrs mie, %0" : : "r" (bits));
            break;
        case CSR_MIP:
            asm volatile ("csrs mip, %0" : : "r" (bits));
            break;
        default:
            break;
    }
}

void InterruptController::clear_csr_bits(uint32_t csr, uint32_t bits) {
    switch (csr) {
        case CSR_MSTATUS:
            asm volatile ("csrc mstatus, %0" : : "r" (bits));
            break;
        case CSR_MIE:
            asm volatile ("csrc mie, %0" : : "r" (bits));
            break;
        case CSR_MIP:
            asm volatile ("csrc mip, %0" : : "r" (bits));
            break;
        default:
            break;
    }
}

void InterruptController::init() {
    if (initialized) return;
    
    // Clear all interrupt enables and pending bits
    write_csr(CSR_MIE, 0);
    write_csr(CSR_MIP, 0);
    
    // Reset statistics
    reset_stats();
    
    initialized = true;
}

void InterruptController::enable_global_interrupts() {
    set_csr_bits(CSR_MSTATUS, MSTATUS_MIE);
}

void InterruptController::disable_global_interrupts() {
    clear_csr_bits(CSR_MSTATUS, MSTATUS_MIE);
}

void InterruptController::enable_machine_timer_interrupt() {
    set_csr_bits(CSR_MIE, MIE_MTIE);
}

void InterruptController::disable_machine_timer_interrupt() {
    clear_csr_bits(CSR_MIE, MIE_MTIE);
}

void InterruptController::enable_machine_software_interrupt() {
    set_csr_bits(CSR_MIE, MIE_MSIE);
}

void InterruptController::disable_machine_software_interrupt() {
    clear_csr_bits(CSR_MIE, MIE_MSIE);
}

void InterruptController::enable_machine_external_interrupt() {
    set_csr_bits(CSR_MIE, MIE_MEIE);
}

void InterruptController::disable_machine_external_interrupt() {
    clear_csr_bits(CSR_MIE, MIE_MEIE);
}

void InterruptController::trigger_software_interrupt() {
    set_csr_bits(CSR_MIP, MIP_MSIP);
}

void InterruptController::clear_software_interrupt() {
    clear_csr_bits(CSR_MIP, MIP_MSIP);
}

void InterruptController::reset_stats() {
    stats.machine_software_count = 0;
    stats.machine_timer_count = 0;
    stats.machine_external_count = 0;
    stats.supervisor_software_count = 0;
    stats.supervisor_timer_count = 0;
    stats.supervisor_external_count = 0;
    stats.unhandled_exception_count = 0;
}

// C-style interrupt handler implementations
extern "C" {

void unhandled_exception_handler() {
    InterruptController::stats.unhandled_exception_count++;
    
    // Read cause and handle accordingly
    uint32_t cause = InterruptController::read_csr(CSR_MCAUSE);
    uint32_t epc = InterruptController::read_csr(CSR_MEPC);
    uint32_t tval = InterruptController::read_csr(CSR_MTVAL);
    
    // In a real system, you might want to log this information
    // For now, we just increment the counter
    (void)cause;
    (void)epc;
    (void)tval;
    
    // Halt the CPU instead of resetting
    // Disable all interrupts to prevent further execution
    InterruptController::disable_global_interrupts();
    
    // Infinite loop with wait-for-interrupt to halt the CPU
    while (1) {
        asm volatile ("wfi"); // Wait for interrupt (low power halt)
    }
}

void machine_software_interrupt_handler() {
    InterruptController::stats.machine_software_count++;
    
    // Clear the software interrupt
    InterruptController::clear_software_interrupt();
}

void machine_timer_interrupt_handler() {
    InterruptController::stats.machine_timer_count++;
    
    // Timer interrupt handling would go here
    // In QEMU virt machine, you might need to clear the timer interrupt
    // by writing to memory-mapped timer registers
}

void machine_external_interrupt_handler() {
    InterruptController::stats.machine_external_count++;
    
    // External interrupt handling (UART, GPIO, etc.) would go here
}

void supervisor_software_interrupt_handler() {
    InterruptController::stats.supervisor_software_count++;
}

void supervisor_timer_interrupt_handler() {
    InterruptController::stats.supervisor_timer_count++;
}

void supervisor_external_interrupt_handler() {
    InterruptController::stats.supervisor_external_count++;
}

}
