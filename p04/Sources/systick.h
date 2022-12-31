//**********************************************************************************************************************
// FILE: systick.h
//
// DESCRIPTION
// Functions to initialize and use the SysTick timer.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#ifndef SYSTICK_H
#define SYSTICK_H

//======================================================================================================================
// Public Types
//======================================================================================================================

// This typedef is used to specify the clock source for the SysTick counter.
typedef enum {
    systick_clk_src_external = 0,
    systick_clk_src_platform = 1
} systick_clk_src_t;

// This type is used to specify if the SysTick interrupt should be disabled or enabled.
typedef enum {
    systick_int_disabled = 0,
    systick_int_enabled  = 1
} systick_int_t;

// This enumerated type specifies the values to load into SYSTICK_RVR in order to obtain a specific SysTick ISR period.
// Note that SYSTICK_RVR[RELOAD] is limited to 24 bits, so the largest value that can be written to RELOAD is 0xFFFFFF
// which would give us a SysTick ISR period of approximately 349.5 ms.
typedef enum {
    systick_period_isr_125_ns =        6,
    systick_period_isr_250_ns =       12,
    systick_period_isr_375_ns =       18,
    systick_period_isr_500_ns =       24,
    systick_period_isr_625_ns =       30,
    systick_period_isr_750_ns =       36,
    systick_period_isr_875_ns =       42,
    systick_period_isr_1_us   =       48,
    systick_period_isr_5_us   =      240,
    systick_period_isr_10_us  =      480,
    systick_period_isr_50_us  =     2400,
    systick_period_isr_100_us =     4800,
    systick_period_isr_250_us =    12000,
    systick_period_isr_500_us =    24000,
    systick_period_isr_1_ms   =    48000,
    systick_period_isr_5_ms   =   240000,
    systick_period_isr_10_ms  =   480000,
    systick_period_isr_25_ms  =  1200000,
    systick_period_isr_50_ms  =  2400000,
    systick_period_isr_100_ms =  4800000,
    systick_period_isr_250_ms = 12000000
} systick_period_isr_t;

// This type us used to specify if the SysTick counter should be disabled or enabled.
typedef enum {
    systick_state_disabled = 0,
    systick_state_enabled  = 1
} systick_state_t;

//======================================================================================================================
// Public Function Declarations
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_callback()
//
// DESCRIPTION
// Configures the callback function that will be called at every SysTick interrupt. p_callback should be 0 if callbacks
// are not requested.
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_callback(callback_vu32_t p_callback);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_clk_src()
//
// DESCRIPTION
// Configures the clock source for the SysTick counter. The choices are: systick_clk_src_external for the external ref-
// erence clock; or systick_clk_src_platform for the platform clock.
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_clk_src(systick_clk_src_t p_clk_src);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_int_state()
//
// DESCRIPTION
// Enables or disables SysTick interrupts.
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_int_state(systick_int_t p_int);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_state()
//
// DESCRIPTION
// Disables or enables the SysTick counter to stop or begin counting.
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_state(systick_state_t p_state);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_init()
//
// DESCRIPTION
// SysTick uses the Platform clock to control the SysTick counter. We have configured the clatform clock to 48 MHz so
// the period is 20.83333... ns. The counter is a down counter, so when the counter is enabled, it will be loaded with
// SYSTICK_RVR[RELOAD] and will decrement once every platform clock. When the counter reaches 0, if configured, it will
// generate an exception, number 15 (IRQ -1). The default SysTick ISR is named systick_isr() in bare_startup.c which
// must be overridden if SysTick interrupts are enabled. We will get a whole number for the SysTick interrupt period
// when SYSTICK_RVR is a multiple of 6. The systick_period_isr_t data type specifies RVR values for common time periods.
//----------------------------------------------------------------------------------------------------------------------
void systick_init(systick_period_isr_t, systick_int_t, systick_state_t, callback_vu32_t);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_read()
//
// DESCRIPTION
// Returns the current value of the counter, i.e, the value of the SYSTICK_CVR register.
//----------------------------------------------------------------------------------------------------------------------
uint32_t systick_read();

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_reset()
//
// DESCRIPTION
// When SysTick has been initialized by calling systick_init() and is either disabled (not counting) or enabled
// (counting and generate IRQs) this function will reset SysTick by: (1) stopping the counter; (2) loading SYST_RVR
// with the desired period; (3) clearing the counter to 0; (4) configuring the callback request; and (5) either disabl-
// ing or enabling the timer based on p_state.
//----------------------------------------------------------------------------------------------------------------------
void systick_reset(systick_period_isr_t p_period, systick_state_t p_state, callback_vu32_t p_callback);

#endif
