//**********************************************************************************************************************
// FILE: systick.c
//
// DESCRIPTION
// See comments in systick.h.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#include "global.h"
#include "int.h"
#include "systick.h"

//======================================================================================================================
// Private Macros
//======================================================================================================================

#define SYST_CSR_CLKSOURCE_SHIFT  2
#define SYST_CSR_ENABLE_SHIFT     0
#define SYST_CSR_TICKINT_SHIFT    1

//======================================================================================================================
// Private Global Variable Definitions
//======================================================================================================================

// Save a pointer to the user's callback function.
static callback_vu32_t g_callback;

//======================================================================================================================
// Function Definitions
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_callback()
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_callback(callback_vu32_t p_callback)
{
    g_callback = p_callback;
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_clk_src()
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_clk_src(systick_clk_src_t p_clk_src)
{
    if (p_clk_src == systick_clk_src_external) {
        SYST_CSR &= ~(1 << SYST_CSR_CLKSOURCE_SHIFT);
    } else {
        SYST_CSR |= 1 << SYST_CSR_CLKSOURCE_SHIFT;
    }
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_int_state()
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_int_state(systick_int_t p_int)
{
    if (p_int == systick_int_disabled) {
        SYST_CSR &= ~(1 << SYST_CSR_TICKINT_SHIFT);
    } else {
        SYST_CSR |= 1 << SYST_CSR_TICKINT_SHIFT;
    }
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_cfg_state()
//----------------------------------------------------------------------------------------------------------------------
void systick_cfg_state(systick_state_t p_state)
{
    if (p_state == systick_state_disabled) {
        SYST_CSR &= ~(1 << SYST_CSR_ENABLE_SHIFT);
    } else {
        SYST_CSR |= 1 << SYST_CSR_ENABLE_SHIFT;
    }
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_init()
//
// DESCRIPTION
// p_period is the time between SysTick interrupt requests (see the systick_period_isr_t typedef in systick.h). p_int
// should be systick_int_enabled if SysTick interrupts are requested. If p_state is systick_state_disabled, the counter
// will not begin counting until enabled later by calling systick_cfg_state(systick_state_enabled). If p_state is
// systick_state_enabled then upon return from this function, the counter will be counting. p_callback is a pointer to
// the user's callback function that will be called at every SysTick interrupt.
//
// NOTE
// The SysTick exception number is 15 (IRQ -1) and has priority 0 by default, which we do not change. This makes it the
// highest priority interrupt in the system (although there are a few exceptions which have higher priority).
//----------------------------------------------------------------------------------------------------------------------
void systick_init(systick_period_isr_t p_period, systick_int_t p_int, systick_state_t p_state, callback_vu32_t p_callback)
{
    // Disable the SysTick counter during initialization.
    systick_cfg_state(systick_state_disabled);

    // Select the platform clock as the clock source.
    systick_cfg_clk_src(systick_clk_src_platform);

    // Reset SysTick. This will: (1) stop the counter; (2) load SYST_RVR with p_period; (3) reset SYST_CVR to 0;
    // (4) enable or disable the counter; and (5) configure the callback function.
    systick_reset(p_period, p_state, p_callback);

    // Enable or disable interrupts based on the value of p_int.
    systick_cfg_int_state(p_int);

    // Configure the disabled/enabled state of the timer to p_state. If p_state is systick_state_enabled, the counter
    // will begin counting now.
    systick_cfg_state(p_state);
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_isr()
//
// DESCRIPTION
// The SysTick ISR which overrides the default systick_isr() function declared in bare_startup.c.
//
// NOTE
// The SysTick interrupt request flag is automatically cleared.
//----------------------------------------------------------------------------------------------------------------------
void systick_isr()
{
    // If requested, call the users callback function, passing the current value of the SysTick counter.
    if (g_callback) {
        g_callback(SYST_CVR);
    }
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_read()
//----------------------------------------------------------------------------------------------------------------------
uint32_t systick_read()
{
    return SYST_CVR;
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: systick_reset()
//----------------------------------------------------------------------------------------------------------------------
void systick_reset(systick_period_isr_t p_period, systick_state_t p_state, callback_vu32_t p_callback)
{
    // Stop the counter.
    systick_cfg_state(systick_state_disabled);

    // Load SYST_RVR[RELOAD] with the proper value such that the desired SysTick ISR period will be obtained.
    SYST_RVR = p_period;

    // Writing any value to SYST_CVR will clear the counter to 0. On the first clock after the counter is enabled to
    // begin counting, it will be loaded with SYST_RVR[RELOAD]. Thus, there will be exactly SYST_RVR[RELOAD] clock ticks
    // before the SysTick exception is generated (when the counter reaches 0).
    SYST_CVR = 0;

    // Configure the callback request.
    systick_cfg_callback(p_callback);

    // Configure the disabled/enabled state of the timer to p_state.
    systick_cfg_state(p_state);
}
