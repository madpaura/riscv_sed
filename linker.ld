/* RISC-V Baremetal Linker Script */

OUTPUT_ARCH(riscv:rv32)
ENTRY(_start)

/* Memory layout with ITCM and DTCM sections */
MEMORY
{
    ITCM (rx)  : ORIGIN = 0x80000000, LENGTH = 64M   /* Instruction Tightly Coupled Memory */
    DTCM (rw)  : ORIGIN = 0x84000000, LENGTH = 64M   /* Data Tightly Coupled Memory */
}

/* Stack size */
STACK_SIZE = 0x10000; /* 64KB stack */

SECTIONS
{
    . = 0x80000000;
    
    /* Text section - executable code in ITCM */
    .text : ALIGN(4)
    {
        KEEP(*(.text.vectors))  /* Interrupt vector table first */
        KEEP(*(.text.start))    /* Startup code */
        *(.text)
        *(.text.*)
        . = ALIGN(4);
    } > ITCM
    
    /* Read-only data in ITCM (close to code for better performance) */
    .rodata : ALIGN(4)
    {
        *(.rodata)
        *(.rodata.*)
        *(.srodata)
        *(.srodata.*)
        . = ALIGN(4);
    } > ITCM
    
    /* Global constructor/destructor tables in ITCM */
    .init_array : ALIGN(4)
    {
        __init_array_start = .;
        KEEP(*(.init_array))
        KEEP(*(SORT(.init_array.*)))
        __init_array_end = .;
    } > ITCM
    
    .fini_array : ALIGN(4)
    {
        __fini_array_start = .;
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.*)))
        __fini_array_end = .;
    } > ITCM
    
    /* Switch to DTCM for data sections */
    . = ORIGIN(DTCM);
    
    /* Initialized data in DTCM */
    .data : ALIGN(4)
    {
        __data_start = .;
        *(.data)
        *(.data.*)
        *(.sdata)
        *(.sdata.*)
        __data_end = .;
    } > DTCM
    
    /* Uninitialized data in DTCM */
    .bss : ALIGN(4)
    {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        *(COMMON)
        __bss_end = .;
    } > DTCM
    
    /* Heap starts after BSS */
    . = ALIGN(4);
    __heap_start = .;
    
    /* Stack at the end of DTCM */
    . = ORIGIN(DTCM) + LENGTH(DTCM);
    . = ALIGN(16);
    __stack_top = .;
    . -= STACK_SIZE;
    __stack_bottom = .;
    
    /* Heap ends where stack begins */
    __heap_end = __stack_bottom;
    
    /* C++ exception handling sections (even if exceptions disabled) */
    .eh_frame : ALIGN(4)
    {
        KEEP(*(.eh_frame))
        . = ALIGN(4);
    } > DTCM
    
    .gcc_except_table : ALIGN(4)
    {
        *(.gcc_except_table)
        *(.gcc_except_table.*)
        . = ALIGN(4);
    } > DTCM
    
    /* Debug sections */
    .debug_info     0 : { *(.debug_info) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_ranges   0 : { *(.debug_ranges) }
    
    /* Discard unwanted sections */
    /DISCARD/ :
    {
        *(.note.GNU-stack)
        *(.gnu_debuglink)
        *(.gnu.lto_*)
    }
}
