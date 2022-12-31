//**********************************************************************************************************************
// FILE: int.h
//
// DESCRIPTION
// The int module handles all functionality associated with interrupt configuration and status.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#ifndef INT_H
#define INT_H

#include "global.h"

//======================================================================================================================
// Public Types
//======================================================================================================================

// Pending status.
typedef enum {
    int_pending     = 0,
    int_not_pending = 1
} int_pending_t;

// Interrupt priority.
typedef enum {
    int_priority_0 = 0b00000000,
    int_priority_1 = 0b01000000,
    int_priority_2 = 0b10000000,
    int_priority_3 = 0b11000000
} int_priority_t;

// This enumerated type specifies the interrupt numbers for various interrupt sources. This is not a complete list of
// all of the interrupt sources, but rather, this is a sublist of the interrupt sources we are most likely to use in
// CSE325.
typedef enum {
    int_src_i2c0    =  8,
    int_src_i2c1    =  9,
    int_src_spi0    = 10,
    int_src_spi1    = 11,
    int_src_uart0   = 12,
    int_src_uart1   = 13,
    int_src_uart2   = 14,
    int_src_adc0    = 15,
    int_src_cmp0    = 16,
    int_src_tpm0    = 17,
    int_src_tpm1    = 18,
    int_src_tpm2    = 19,
    int_src_pit     = 22,
    int_src_dac0    = 25,
    int_src_tsi0    = 26,
    int_src_slcd    = 29,
    int_src_porta   = 30,
    int_src_portscd = 31
} int_src_t;

// Interrupt enabled/disabled state.
typedef enum {
    int_state_disabled = 0,
    int_state_enabled  = 1
} int_state_t;

//======================================================================================================================
// Public Function Declarations
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_cfg_pending_state()
//
// DESCRIPTION
// Configures the pending state of the interrupt src to either pending or not pending.
//----------------------------------------------------------------------------------------------------------------------
void int_cfg_pending_state(int_src_t p_src, int_pending_t p_pending_state);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_cfg_priority()
//
// DESCRIPTION
// Configures the priority of the interrupt src to 0, 1, 2, or 3.
//----------------------------------------------------------------------------------------------------------------------
void int_cfg_priority(int_src_t p_src, int_priority_t p_priority);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_cfg_state()
//
// DESCRIPTION
// Configures the state of the interrupt source to either enabled or disabled.
//----------------------------------------------------------------------------------------------------------------------
void int_cfg_state(int_src_t p_src, int_state_t p_state);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_get_pending_state()
//
// DESCRIPTION
// Returns the pending state (pending or not pending) of the interrupt source.
//----------------------------------------------------------------------------------------------------------------------
int_pending_t int_get_pending_state(int_src_t p_src);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_get_priority()
//
// DESCRIPTION
// Returns the priority of the interrupt source.
//----------------------------------------------------------------------------------------------------------------------
int int_get_priority(int_src_t p_src);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_get_state()
//
// DESCRIPTION
// Returns the state (either enabled or disabled) of the interrupt source.
//----------------------------------------------------------------------------------------------------------------------
int_state_t int_get_state(int_src_t p_src);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_init()
//
// DESCRIPTION
// Initializes interrupt source p_src. Configures the priority to be p_priority. Enables or disables the interrupt state
// using the value in p_state.
//
// NOTES
// The name of the ISR function is hardcoded in the bare_vector_table[] array defined in bare_startup.c. This name must
// be used to override the default ISR that is defined to be weak (meaning it can be overridden) and which is directed
// to bare_default_isr(). Also, we have to comment out the weak declaration in bare_startup.c for some reason I do not
// understand (we should not have to, as weak implies that the function can be overridden).
//----------------------------------------------------------------------------------------------------------------------
void int_init(int_src_t p_src, int_priority_t p_priority, int_state_t p_state);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_mask_all()
//
// DESCRIPTION
// Masks all interrupt sources by setting PRIMASK[PM].
//----------------------------------------------------------------------------------------------------------------------
void int_mask_all();

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_unmask_all()
//
// DESCRIPTION
// Unmasks  all interrupt sources by clearing PRIMASK[PM].
//----------------------------------------------------------------------------------------------------------------------
void int_unmask_all();

#endif
