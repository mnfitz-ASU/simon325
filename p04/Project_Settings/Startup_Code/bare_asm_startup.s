@***********************************************************************************************************************
@ FILE: bare_asm_startup.s
@
@ DESCRIPTION
@ Contains the startup routine, bare_startup, which calls various functions to initialize stuff and then calls main().
@ main() should never return but in case it does, we just drop into an infinite loop so the program does not just
@ crash.
@
@ AUTHOR
@ Kevin R. Burger (burgerk@asu.edu)
@ Computer Science & Engineering
@ Arizona State University, Tempe AZ 85287-8809
@***********************************************************************************************************************

.file       "bare_asm__startup.s"  @ Provides the name of the source file to the debugger
.syntax     unified                @ Use the ARM unified assemmbler syntax [Ref: as.pdf, Sect 9.4.2.1]

@=======================================================================================================================
@ Public Labels to be Imported
@=======================================================================================================================

.extern __init_sp                      @ Initial address of stack pointer register
.extern bare_hw_init                   @ bare_hw_init() defined in bare_startup.c
.extern bare_bss_zero_fill             @ bare_bss_zero_fill() defined in bare_startup.c
.extern bare_copy_rom_sections_to_ram  @ bare_copy_rom_sections_to_ram() defined in bare_startup.c
.extern main                           @ main() defined somewhere, usually in main.c

@=======================================================================================================================
@ Public Labels to be Exported
@=======================================================================================================================

.global bare_sp_init  @ Initial value for the sp register
.global bare_startup  @ bare_startup() must be accessible in the linker command file

@=======================================================================================================================
@ SECTION: .text
@=======================================================================================================================
.text

@-----------------------------------------------------------------------------------------------------------------------
@ FUNCTION: static void bare_sp_init()
@
@ DESCRIPTION
@ Writes a bottom-of-stack marker (0xDEADBEEF) at the last word of SRAM (0x20005FFC) and initializes the stack pointer
@ register to that address.
@
@ REGISTER USAGE
@ r0-r1, sp
@-----------------------------------------------------------------------------------------------------------------------
.func bare_sp_init, bare_sp_init
.type bare_sp_init, %function
.thumb_func
bare_sp_init:
    ldr     r0, =__init_sp              @ r0 = memory address where SP register is initialized to point to
    ldr     r1, dead_beef               @ r1 = 0xDEAD_BEEF
    str     r1, [r0]                    @ MEM[__init_sp] = 0xDEADBEEF
    mov     sp, r0                      @ sp = __init_sp
    bx      lr                          @ return
.align  2
dead_beef:  .word 0xDEADBEEF
.endfunc

@-----------------------------------------------------------------------------------------------------------------------
@ FUNCTION: static void bare_startup()
@
@ DESCRIPTION
@ When the board is powered up or when we start executing our program in an IDE, bare_startup() is the entry point of
@ execution. Think of it as the assembly language version of main(). We call various functions to initialize things and
@ then call main(). Note that main() should not return, but if it does we drop into an infinite loop to keep the system
@ from just crashing.
@
@ REGISTER USAGE
@ None
@
@ REMARK
@ Technically, according to the C language standard, main() should be capable of receiving arguments but we are not
@ passing any arguments to main() so that is that.
@-----------------------------------------------------------------------------------------------------------------------
.func bare_startup, bare_startup
.type bare_startup, %function
.thumb_func
bare_startup:
    bl      bare_sp_init                   @ Call bare_sp_init()
    bl      bare_hw_init                   @ Call bare_hw_init()
    bl      bare_bss_zero_fill             @ Call bare_bss_zero_fill()
    bl      bare_copy_rom_sections_to_ram  @ Call bare_copy_rom_sections_to_ram()
    bl      main                           @ Call main()

@ Drop into an infinite loop if main() should return.
@ This is equivalent to: for(;;);
1:  b       1b
.endfunc
