//************************************************************************************************************
// File: main.c
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
// nxp
#include "global.h" // Access to int data types and includes mkl46z.h

// Project 4
#include "gpio.h"
#include "led.h"
#include "port.h"
#include "sim.h"
#include "tpm.h"

#include "pit.h"
#include "int.h"
#include "pushb.h"
#include "rand.h"
#include "simon.h"
#include "systick.h"

// A preprocessor macro used to construct an infinite loop.
#define forever for(;;)
// In this project:

// Main features of this project:


int main()
{
	// Initialize on startup
	SimInitialize(SimGet());
	
	GpioInitialize(GpioGet(kGpioA));
	GpioInitialize(GpioGet(kGpioB));
	GpioInitialize(GpioGet(kGpioD));
	
	PortInitialize(PortGet(kPortA));
	PortInitialize(PortGet(kPortB));
	PortInitialize(PortGet(kPortD));
	
	// We assume that for Project 4, we will always use TPM0 for delay
	Tpm* tpm = TpmGet(kTimer0);
	TpmInitialize(tpm);
	
	uint32_t ticks = 0;
	UNUSED(ticks);
	for (;;)
	{
		// Play Simon forever
		SimonPlay();
	}
	
	TpmFinalize(tpm);
	
	PortFinalize(PortGet(kPortD));
	PortFinalize(PortGet(kPortB));
	PortFinalize(PortGet(kPortA));
	
	GpioFinalize(GpioGet(kGpioD));
	GpioFinalize(GpioGet(kGpioB));
	GpioFinalize(GpioGet(kGpioA));
		
	SimFinalize(SimGet());
	   
    // main() should never return, but C requires that main() return an int, so here it is.
    return 0;

}
