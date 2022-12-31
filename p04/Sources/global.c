//**********************************************************************************************************************
// FILE: global.c
//
// DESCRIPTION
// Global definitions for stuff declared in global.h.
//
// AUTHOR
// Kevin R. Burger (burgerk@asu.edu)
// Computer Science & Engineering
// Arizona State University, Tempe AZ 85287-8809
// Web: http://www.devlang.com
//**********************************************************************************************************************
#include "global.h"

uint32_t const GLOBAL_FREQ_EXT_CRYSTAL_HZ  = 8000000;
uint32_t const GLOBAL_FREQ_EXT_CRYSTAL_KHZ = 8000;
uint32_t const GLOBAL_FREQ_EXT_CRYSTAL_MHz = 8;

// These global variables are initialized in bare_clocks_init() which is in bare_startup.c.
uint32_t global_bus_clock_freq_mhz;
uint32_t global_core_clock_freq_mhz;
uint32_t global_flash_clock_freq_mhz;
uint32_t global_mcgirclk_freq_mhz;
uint32_t global_platform_clock_freq_mhz;
uint32_t global_oscerclk_freq_mhz;
uint32_t global_system_clock_freq_mhz;
