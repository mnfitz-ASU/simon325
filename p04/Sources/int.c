//**********************************************************************************************************************
// FILE: int.c
//
// DESCRIPTION
// See comments in int.c.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#include "int.h"

//======================================================================================================================
// Public and Private Function Definitions
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_cfg_pending_state()
//----------------------------------------------------------------------------------------------------------------------
void int_cfg_pending_state(int_src_t p_src, int_pending_t p_pending_state)
{
    if (p_pending_state == int_pending) {
        NVIC_ISPR |= 1 << p_src;
    } else {
        NVIC_ICPR |= 1 << p_src;
    }
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_cfg_priority()
//----------------------------------------------------------------------------------------------------------------------
void int_cfg_priority(int_src_t p_src, int_priority_t p_priority)
{
    int ipr_reg_num = p_src >> 2;               // ipr_reg_num is number of the NVIC_IPR register
    int bit_pos = (p_src % 4) << 3;             // bit_pos is the least-significant bit of the proper PRI_xx field
    uint32_t nvic_iprn = NVIC_IP(ipr_reg_num);  // nvic_iprn is the current value of NVIC_IPR<ipr_reg_num>
    nvic_iprn &= ~(0b11111111 << bit_pos);      // Clear the PRI_xx field
    nvic_iprn |= p_priority << bit_pos;         // Write the priority into the PRI_xx field
    NVIC_IP(ipr_reg_num) = nvic_iprn;           // Update NVIC_IPR<ipr_reg_num>
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_cfg_state()
//----------------------------------------------------------------------------------------------------------------------
void int_cfg_state(int_src_t p_src, int_state_t p_state)
{
    if (p_state == int_state_enabled) {
        NVIC_ISER |= 1 << p_src;
    } else {
        NVIC_ICER |= 1 << p_src;
    }
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_get_pending_state()
//----------------------------------------------------------------------------------------------------------------------
int_pending_t int_get_pending_state(int_src_t p_src)
{
    return (NVIC_ISPR & (1 << p_src)) == 0 ? int_not_pending : int_pending;
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_get_priority()
//----------------------------------------------------------------------------------------------------------------------
int int_get_priority(int_src_t p_src)
{
    int ipr_reg_num = p_src >> 2;    // ipr_reg_num is number of the NVIC_IPR register
    int bit_pos = (p_src % 4) << 3;  // bit_pos is the least-significant bit of the proper PRI_xx field
    return (NVIC_IP(ipr_reg_num) & (0b11000000 << bit_pos)) >> (bit_pos + 6);
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_get_state()
//----------------------------------------------------------------------------------------------------------------------
int_state_t int_get_state(int_src_t p_src)
{
    return (NVIC_ISER & (1 << p_src)) == 0 ? int_state_disabled : int_state_enabled;
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_init()
//----------------------------------------------------------------------------------------------------------------------
void int_init(int_src_t p_src, int_priority_t p_priority, int_state_t p_state)
{
    int_cfg_priority(p_src, p_priority);   // Configure the priority
    int_cfg_state(p_src, p_state);         // Configure the state to be enabled or disabled
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_mask_all()
//----------------------------------------------------------------------------------------------------------------------
void int_mask_all()
{
    __asm("cpsid i");
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: int_unmask_all()
//----------------------------------------------------------------------------------------------------------------------
void int_unmask_all()
{
    __asm("cpsie i");
}
