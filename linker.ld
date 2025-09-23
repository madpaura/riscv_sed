/* RISC-V Baremetal Linker Script */

OUTPUT_ARCH(riscv:rv32)
ENTRY(_start)

/* Memory layout for QEMU virt machine */
MEMORY
{
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

/* Stack size */
STACK_SIZE = 0x10000; /* 64KB stack */

SECTIONS
{
    . = 0x80000000;
    
    /* Text section - executable code */
    .text : ALIGN(4)
    {
        KEEP(*(.text.vectors))  /* Interrupt vector table first */
        KEEP(*(.text.start))    /* Startup code */
        *(.text)
        *(.text.*)
        . = ALIGN(4);
    } > RAM
    
    /* Read-only data */
    .rodata : ALIGN(4)
    {
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
    } > RAM
    
    /* Global constructor/destructor tables */
    .init_array : ALIGN(4)
    {
        __init_array_start = .;
        KEEP(*(.init_array))
        KEEP(*(SORT(.init_array.*)))
        __init_array_end = .;
    } > RAM
    
    .fini_array : ALIGN(4)
    {
        __fini_array_start = .;
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.*)))
        __fini_array_end = .;
    } > RAM
    
    /* Initialized data */
    .data : ALIGN(4)
    {
        __data_start = .;
        *(.data)
        *(.data.*)
        __data_end = .;
    } > RAM
    
    /* Uninitialized data */
    .bss : ALIGN(4)
    {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __bss_end = .;
    } > RAM
    
    /* Heap starts after BSS */
    . = ALIGN(4);
    __heap_start = .;
    
    /* Stack at the end of RAM */
    . = ORIGIN(RAM) + LENGTH(RAM);
    . = ALIGN(16);
    __stack_top = .;
    . -= STACK_SIZE;
    __stack_bottom = .;
    
    /* Heap ends where stack begins */
    __heap_end = __stack_bottom;
    
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
