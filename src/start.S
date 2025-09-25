/* RISC-V Startup Assembly Code */

.section .text.start
.global _start

/* Interrupt Vector Table */
.section .text.vectors
.align 2
.global _vector_table
_vector_table:
    j _start                    /* Reset */
    j _unhandled_exception      /* Supervisor software interrupt */
    j _unhandled_exception      /* Reserved */
    j _machine_software_int     /* Machine software interrupt */
    j _unhandled_exception      /* Reserved */
    j _supervisor_timer_int     /* Supervisor timer interrupt */
    j _unhandled_exception      /* Reserved */
    j _machine_timer_int        /* Machine timer interrupt */
    j _unhandled_exception      /* Reserved */
    j _supervisor_external_int  /* Supervisor external interrupt */
    j _unhandled_exception      /* Reserved */
    j _machine_external_int     /* Machine external interrupt */

.section .text.start
_start:
    /* Disable interrupts initially */
    csrw mie, zero
    csrw mip, zero
    
    /* Set up machine trap vector */
    la t0, _vector_table
    csrw mtvec, t0
    
    /* Set up stack pointer */
    la sp, __stack_top
    
    /* Clear BSS section */
    la t0, __bss_start
    la t1, __bss_end
clear_bss:
    beq t0, t1, bss_done
    sw zero, 0(t0)
    addi t0, t0, 4
    j clear_bss
bss_done:

    /* Call global constructors */
    call __call_constructors
    
    /* Jump to main */
    call main
    
    /* Call global destructors */
    call __call_destructors
    
    /* Halt the system */
halt:
    wfi
    j halt
/* Function to call global constructors */
__call_constructors:
    la t0, __init_array_start
    la t1, __init_array_end
constructor_loop:
    beq t0, t1, constructor_done
    lw t2, 0(t0)
    beqz t2, skip_constructor
    jalr t2
skip_constructor:
    addi t0, t0, 4
    j constructor_loop
constructor_done:
    ret

/* Function to call global destructors */
__call_destructors:
    la t0, __fini_array_start
    la t1, __fini_array_end
destructor_loop:
    beq t0, t1, destructor_done
    lw t2, 0(t0)
    beqz t2, skip_destructor
    jalr t2
skip_destructor:
    addi t0, t0, 4
    j destructor_loop
destructor_done:
    ret

/* Interrupt Service Routines */

/* Unhandled exception handler */
_unhandled_exception:
    /* Save context */
    addi sp, sp, -64
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw t1, 8(sp)
    sw t2, 12(sp)
    sw a0, 16(sp)
    sw a1, 20(sp)
    sw a2, 24(sp)
    sw a3, 28(sp)
    sw a4, 32(sp)
    sw a5, 36(sp)
    sw a6, 40(sp)
    sw a7, 44(sp)
    
    /* Call C handler */
    call unhandled_exception_handler
    
    /* Restore context */
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw t1, 8(sp)
    lw t2, 12(sp)
    lw a0, 16(sp)
    lw a1, 20(sp)
    lw a2, 24(sp)
    lw a3, 28(sp)
    lw a4, 32(sp)
    lw a5, 36(sp)
    lw a6, 40(sp)
    lw a7, 44(sp)
    addi sp, sp, 64
    mret

/* Machine software interrupt handler */
_machine_software_int:
    addi sp, sp, -64
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw t1, 8(sp)
    sw t2, 12(sp)
    sw a0, 16(sp)
    sw a1, 20(sp)
    sw a2, 24(sp)
    sw a3, 28(sp)
    sw a4, 32(sp)
    sw a5, 36(sp)
    sw a6, 40(sp)
    sw a7, 44(sp)
    
    call machine_software_interrupt_handler
    
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw t1, 8(sp)
    lw t2, 12(sp)
    lw a0, 16(sp)
    lw a1, 20(sp)
    lw a2, 24(sp)
    lw a3, 28(sp)
    lw a4, 32(sp)
    lw a5, 36(sp)
    lw a6, 40(sp)
    lw a7, 44(sp)
    addi sp, sp, 64
    mret

/* Supervisor timer interrupt handler */
_supervisor_timer_int:
    addi sp, sp, -64
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw t1, 8(sp)
    sw t2, 12(sp)
    sw a0, 16(sp)
    sw a1, 20(sp)
    sw a2, 24(sp)
    sw a3, 28(sp)
    sw a4, 32(sp)
    sw a5, 36(sp)
    sw a6, 40(sp)
    sw a7, 44(sp)
    
    call supervisor_timer_interrupt_handler
    
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw t1, 8(sp)
    lw t2, 12(sp)
    lw a0, 16(sp)
    lw a1, 20(sp)
    lw a2, 24(sp)
    lw a3, 28(sp)
    lw a4, 32(sp)
    lw a5, 36(sp)
    lw a6, 40(sp)
    lw a7, 44(sp)
    addi sp, sp, 64
    mret

/* Machine timer interrupt handler */
_machine_timer_int:
    addi sp, sp, -64
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw t1, 8(sp)
    sw t2, 12(sp)
    sw a0, 16(sp)
    sw a1, 20(sp)
    sw a2, 24(sp)
    sw a3, 28(sp)
    sw a4, 32(sp)
    sw a5, 36(sp)
    sw a6, 40(sp)
    sw a7, 44(sp)
    
    call machine_timer_interrupt_handler
    
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw t1, 8(sp)
    lw t2, 12(sp)
    lw a0, 16(sp)
    lw a1, 20(sp)
    lw a2, 24(sp)
    lw a3, 28(sp)
    lw a4, 32(sp)
    lw a5, 36(sp)
    lw a6, 40(sp)
    lw a7, 44(sp)
    addi sp, sp, 64
    mret

/* Supervisor external interrupt handler */
_supervisor_external_int:
    addi sp, sp, -64
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw t1, 8(sp)
    sw t2, 12(sp)
    sw a0, 16(sp)
    sw a1, 20(sp)
    sw a2, 24(sp)
    sw a3, 28(sp)
    sw a4, 32(sp)
    sw a5, 36(sp)
    sw a6, 40(sp)
    sw a7, 44(sp)
    
    call supervisor_external_interrupt_handler
    
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw t1, 8(sp)
    lw t2, 12(sp)
    lw a0, 16(sp)
    lw a1, 20(sp)
    lw a2, 24(sp)
    lw a3, 28(sp)
    lw a4, 32(sp)
    lw a5, 36(sp)
    lw a6, 40(sp)
    lw a7, 44(sp)
    addi sp, sp, 64
    mret

/* Machine external interrupt handler */
_machine_external_int:
    addi sp, sp, -64
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw t1, 8(sp)
    sw t2, 12(sp)
    sw a0, 16(sp)
    sw a1, 20(sp)
    sw a2, 24(sp)
    sw a3, 28(sp)
    sw a4, 32(sp)
    sw a5, 36(sp)
    sw a6, 40(sp)
    sw a7, 44(sp)
    
    call machine_external_interrupt_handler
    
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw t1, 8(sp)
    lw t2, 12(sp)
    lw a0, 16(sp)
    lw a1, 20(sp)
    lw a2, 24(sp)
    lw a3, 28(sp)
    lw a4, 32(sp)
    lw a5, 36(sp)
    lw a6, 40(sp)
    lw a7, 44(sp)
    addi sp, sp, 64
    mret
