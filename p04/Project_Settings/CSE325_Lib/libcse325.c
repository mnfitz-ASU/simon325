//**********************************************************************************************************************
// FILE: libcse325.c
//
// DESCRIPTION
// Contains useful functions that may be used in place of those from the C Standard Library since we are not using it.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//**********************************************************************************************************************
#include "libcse325.h"

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: cse325_memcpy()
//
// DESCRIPTION
// See comments in libcse325.h.
//----------------------------------------------------------------------------------------------------------------------
void cse325_memcpy(uint8_t *p_dst, uint8_t *p_src, int p_size)
{
    if (p_dst == p_src || !p_dst || !p_src) return;
    for ( ; p_size > 0; --p_size) {
        *p_dst++ = *p_src++;
    }
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: cse325_memset()
//
// DESCRIPTION
// See comments in libcse325.h.
//----------------------------------------------------------------------------------------------------------------------
void cse325_memset(uint8_t *p_begin, uint8_t p_fill_value, int p_size)
{
    if (!p_begin) return;
    for ( ; p_size > 0; --p_size) {
        *p_begin++ = p_fill_value;
    }
}
