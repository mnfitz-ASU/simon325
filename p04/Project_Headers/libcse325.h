//**********************************************************************************************************************
// FILE: libcse325.h
//
// DESCRIPTION
// Declarations for stuff defined in libcse325.h.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//**********************************************************************************************************************
#ifndef LIBCSE325_H
#define LIBCSE325_H

#include "global.h"

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: cse325_memcpy()
//
// DESCRIPTION
// This is the standard memcpy() function from the C Standard Library. Copy a block of p_size bytes starting at p_src
// to p_dst. Does not perform any error checking to ensure that the destination block is large enough to store the
// copied bytes.
//----------------------------------------------------------------------------------------------------------------------
void cse325_memcpy(uint8_t *p_dst, uint8_t *p_src, int p_size);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: cse325_memset()
//
// DESCRIPTION
// This is the standard memset() function from the C Standard Library. Fill a block of memory of size p_size starting
// at address p_begin with the value p_fill_value.
//----------------------------------------------------------------------------------------------------------------------
void cse325_memset(uint8_t *p_begin, uint8_t p_fill_value, int p_size);

#endif
