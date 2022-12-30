//************************************************************************************************************
// File: sim.c
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#include "sim.h"

// nxp
#include "global.h"

// project 2
#include "port.h"

// c99
#include <stdbool.h>
#include <string.h>

#define UNUSED(var) (void)(var)

// Structure that defines an instance of the System Integration Module device
struct Sim
{
	// has-a: base address
	SIM_MemMapPtr mBaseAddress; // Stolen from mkl46z.h
	// has-a: boolean
	bool mIsInitialized;
};

// Here is the global sim instance
Sim gSim = {SIM_BASE_PTR, false};

Sim* SimGet()
{
	return &gSim;
}

void SimInitialize(Sim* ioSim)
{
	// Dangerous: Assumes that only Ports A, B, and D will be used in this project
	const uint32_t kMaskAll = (SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK);
	SimSetRegisterSCGC5(ioSim, kMaskAll);
	ioSim->mIsInitialized = true;
}

void SimFinalize(Sim* ioSim)
{
	ioSim->mIsInitialized = false;
}

uint32_t SimGetRegisterSCGC5(const Sim* inSim)
{
	const uint32_t value = SIM_SCGC5_REG(inSim->mBaseAddress);
	return value;
}

void SimSetRegisterSCGC5(Sim* ioSim, uint32_t inValue)
{
	SIM_SCGC5_REG(ioSim->mBaseAddress) = inValue;
}

uint32_t SimGetRegisterSCGC6(const Sim* inSim)
{
	const uint32_t value = SIM_SCGC6_REG(inSim->mBaseAddress);
	return value;
}

void SimSetRegisterSCGC6(Sim* ioSim, uint32_t inValue)
{
	SIM_SCGC6_REG(ioSim->mBaseAddress) = inValue;
}

uint32_t SimGetRegisterSOPT2(const Sim* inSim)
{
	const uint32_t value = SIM_SOPT2_REG(inSim->mBaseAddress);
	return value;
}

void SimSetRegisterSOPT2 (Sim* ioSim, uint32_t inValue)
{
	SIM_SOPT2_REG(ioSim->mBaseAddress) = inValue;
}

void* SimGetBaseAddress(const Sim* inSim)
{
	//return inSim->mBaseAddress;
	UNUSED(inSim);
	return NULL;
}




