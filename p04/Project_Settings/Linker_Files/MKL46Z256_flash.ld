/***********************************************************************************************************************
 * FILE: MKL46Z256_flash.ld
 *
 * DESCRIPTION
 * Linker command script. Based on the default linker command script with the following modifications:
 *
 * AUTHOR
 * Kevin Burger (burgerk@asu.edu)
 * Computer Science & Engineering
 * Arizona State University, Tempe, AZ 85287-8809
 **********************************************************************************************************************/

/* The ENTRY command specifies a label or function which is the starting point of execution. bare_startup() is
 * defined in bare_startup.s, so when the board is powered up and comes out of reset, the instruction pointer register
 * will be initialized with the address of bare_startup() and this will cause execution of our code to begin there.
 */ 
ENTRY(bare_startup)

/* The last byte of the SRAM is at 0x2000_5FFF and since the stack grows downward (we say it is a descending stack) the
 * stack pointer register is initialized in the startup code to 0x2000_5FFC. The startup code places a marker here,
 * 0xDEAD_BEEF, to represent the bottom of the stack. __init_sp	is used in the startup code as the value to initialize
 * the SP register to. We do not place a limit on the size of the stack. It will grow downward from the top of SRAM and
 * global data will be stored in SRAM starting at the bottom of SRAM. If a program is not careful, and it uses too much
 * stack space, then it could begin to overwrite global data. There are at least a couple of ways to handle this. The
 * first way is to pretend it will not happen. The second way would be to define the size of the stack, thus defining
 * the last address in memory of the top of the stack, and then monitor the size of the stack with either a periodic
 * task or check for stack overflow at the beginning of each function. We are going to take the easy way out and pretend
 * we will not have stack overflow. */
 __init_sp = 0x20006000 - 4; 

/* The MEMORY command specifies memory areas where sections (see the SECTIONS command) will be mapped to. */
MEMORY
{
    /* The m_interrupts section is at [0x0000_0000, 0x0000_00C0) which is in the flash region. It stores the 48-entry
     * exception vector table. Since each entry in the vector table is a 4-byte function address, the size of the
     * m_interrupts segment is 48 x 4 = 192 = 0xC0 bytes. */
    m_interrupts (rx) : ORIGIN = 0x00000000, LENGTH = 0xC0

    /* This is the memory segment for the 16-byte Flash Configuration Field (see KL46-SRM Sect 27.3.1). This 16-byte
     * memory region is at addresses [0x0000_0400, 0x0000_040C). By default, all of these 16-bytes are erased and contain
     * the values 0xFF. */
    m_cfmprotrom (rx) : ORIGIN = 0x00000400, LENGTH = 0x10

    /* The m_text section is at [0x0000_0800, 0x0040_0000) which is in the flash region. It stores program instructions
     * and read-only data (global initialized constants). */
    m_text (rx) : ORIGIN = 0x00000800, LENGTH = 256K - 0x800

    /* The m_data section is at [0x1FFF_E000, 0x2000_6000) which is the entire SRAM region. It stores global initialized
     * and uninitialized data, and the stack. */
    m_data (rwx) : ORIGIN = 0x1FFFE000, LENGTH = 32K
}

/* The SECTIONS command specifies memory sections and their contents (which come from sections specified in .o files).
 * These sections subsequently are mapped to the memory segments defined in the MEMORY area above. */
SECTIONS
{
    /* Place the exception vector table (.vectortable; see __bare_startup.c) in the m_interrupt region. This means the
     * table will occupy memory addresses [0x0000_0000, 0x0000_00C0). */
    .interrupts :
    {*(.vectorable)
        __vector_table = .;    /* Define global variable __vector_table equal to the address of .vectortable */
        . = ALIGN(4);
        KEEP(*(.vectortable))  /* KEEP means do not dead strip the .vectortable section */
        . = ALIGN(4);
    } > m_interrupts
  
    /* I believe the flash configuration field section has something to do with protecting certain sections of the
	 * flash memory from being accessible in normal mode when running a program. This is necessary to prevent errant
	 * code from overwriting the bootloader code which is stored in a protected flash memory region because if the 
	 * bootloader code is overwritten, then the chip will not be able to boot up and then one would have a bricked
	 * FRDM-KL46Z board. */
    .cfmprotect :
    {
        . = ALIGN(4);
        KEEP(*(.cfmconfig))	/* Do not dead strip the Flash Configuration Field (FCF) */
        . = ALIGN(4);
    } > m_cfmprotrom

    /* Program instructions and read-only data (constants) are mapped to the m_text memory segment which is in flash. */ 
    .text :
    {
        . = ALIGN(4);
        *(.text)        /* All .text sections containing code */
        *(.text*)       /* All .text* sections containing code */  
        *(.rodata)      /* All .rodata sections (constants, strings literals, etc.) */
        *(.rodata*)     /* All .rodata* sections (constants, strings iterals, etc.) */
        . = ALIGN(4);
        ___ROM_AT = .;  /* __ROM_AT is the address in flash where instructions and constants end and init's data begins. */
    } > m_text

    /* Initialized global data is mapped to the m_data memory segment, which places it in SRAM. During startup, this
     * data will be copied from ROM to SRAM by the __bare_copy_rom_sections_to_ram() function. */
    .data : AT(___ROM_AT)
    {
        . = ALIGN(4);
        __data_begin = .;  /* Variable equal to the beginning of the .data section */ 
        *(.data)           /* .data sections */
        *(.data*)          /* .data* sections */
        . = ALIGN(4);
        __data_end = .;    /* Variable equal to the end of the .data section */
    } > m_data

    /* Set ___data_size to the size of the .data section, in bytes. */  
    __data_size = __data_end - __data_begin;
    
    /* The .bss section contains unintialized global data. It will be allocated in SRAM and initialized to all 0's
     * during startup. See __bare_bss_zero_fill in __bare_startup.c. */
    . = ALIGN(4);
    .bss :
    {
        PROVIDE(__bss_begin = .); /* The beginning of the .bss section */
        *(.bss)                   /* All .bss sections */
        *(.bss*)                  /* All .bss* sections */
        *(COMMON)                 /* All COMMON sections */
        . = ALIGN(4);
        PROVIDE(__bss_end = .);   /* The end of the .bss section */
    } > m_data

    /* The ROM pointer table begins in memory following the end of the .data section. */
    __romp_at = ___ROM_AT + SIZEOF(.data);

    .romp : AT(__romp_at)
    {
        __romp = __romp_at;   /* Define a variable __romp which is an array and init'd to the following data */
        LONG(___ROM_AT);      /* Target destination in SRAM. */
        LONG(__data_begin);   /* Source in flash. */
        LONG(__data_size);    /* Number of bytes to copy from ROM to RAM. */
        LONG(0);              /* The ROM pointer table is terminated with a struct entry where all of the data */
        LONG(0);              /* members of the struct are 0. */
        LONG(0);
    } > m_data                /* Send the ROM pointer out to m_data memory segment. */
}
