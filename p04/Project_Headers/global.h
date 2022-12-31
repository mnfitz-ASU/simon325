//**********************************************************************************************************************
// FILE: global.h
//
// DESCRIPTION
// This file will be included in just about every source code file. It does this:
//
// 1. Defines useful typedefs for different sized ints.
// 2. Defines a bool_t data type.
// 3. Declares constants for the clock frequency of the external crystal.
// 4. Declares global variables for the clock frequencies of various clock sources.
// 5. Includes mkl46z.h to give us access to all of the register and bit mask declarations.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
//**********************************************************************************************************************
#ifndef GLOBAL_H
#define GLOBAL_H

// Signed int data types.
typedef signed char         int8_t;
typedef short int           int16_t;
typedef long int            int32_t;

// Unsigned int data types.
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned long int   uint32_t;

// Simulates the bool data type that C does not have.
typedef enum {
    false = 0,
    true = 1,
} bool_t;

// These constants specify the clock frequency of the external crystal.
extern uint32_t const GLOBAL_FREQ_EXT_CRYSTAL_HZ;
extern uint32_t const GLOBAL_FREQ_EXT_CRYSTAL_KHZ;
extern uint32_t const GLOBAL_FREQ_EXT_CRYSTAL_MHZ;

// These variables store the clock frequencies of various clock sources.
extern uint32_t global_bus_clock_freq_mhz;
extern uint32_t global_flash_clock_freq_mhz;
extern uint32_t global_core_clock_freq_mhz;
extern uint32_t global_mcgirclk_freq_mhz;
extern uint32_t global_platform_clock_freq_mhz;
extern uint32_t global_oscerclk_freq_mhz;
extern uint32_t global_system_clock_freq_mhz;

// Include the CW "primary" header file for the definitions of the peripheral registers and bit
// masks for manipulating individual bits in the registers.
#include "mkl46z.h"

#endif
