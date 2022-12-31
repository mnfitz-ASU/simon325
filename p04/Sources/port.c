//************************************************************************************************************
// File: port.c
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************#include "port.h"

// project 4
#include "port.h"

// nxp
#include "global.h"

// c99
#include <stdbool.h>
#include <string.h>

#define UNUSED(var) (void)(var)

struct Port
{
	//void* mBaseAddress;
	PortType mPortType;
	PORT_MemMapPtr mBaseAddress; // Stolen from mkl46z.h
	bool mIsInitialized;
};

Port gPortA = {kPortA, PORTA_BASE_PTR, false}; // External J1 PushBs
Port gPortB = {kPortB, PORTB_BASE_PTR, false}; // External J4 LEDs
Port gPortD = {kPortD, PORTD_BASE_PTR, false}; // Internal LED

Port* PortGet(PortType inPortType)
{
	switch (inPortType)
	{
	case kPortA:
		return &gPortA;
	case kPortB:
		return &gPortB;
	case kPortD:
		return &gPortD;
	default:
		//assert(!"Invalid PortType selected!"); // How did we get here?
		return NULL;
	}
}

void PortInitialize(Port* ioPort)
{
	ioPort->mIsInitialized = true;
}

void PortFinalize(Port* ioPort)
{
	ioPort->mIsInitialized = false;
}

void* PortGetBaseAddress(const Port* inPort)
{
	//return inPort->mBaseAddress;
	UNUSED(inPort);
	return NULL;
}

uint32_t PortGetRegisterPCR(const Port* inPort, int inPinNumber)
{	
	const uint32_t value = PORT_PCR_REG(inPort->mBaseAddress, inPinNumber); 
	return value;
}

void PortSetRegisterPCR(Port* inPort, int inPinNumber, uint32_t inValue)
{
	PORT_PCR_REG(inPort->mBaseAddress, inPinNumber) = inValue;
}





