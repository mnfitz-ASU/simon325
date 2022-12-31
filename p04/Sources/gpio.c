//************************************************************************************************************
// File: gpio.c
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#include "gpio.h"

// nxp
#include "global.h"

// c99
#include <stdbool.h>
#include <string.h>

#define UNUSED(var) (void)(var)

struct Gpio
{
	// has-a: base address
	GPIO_MemMapPtr mBaseAddress; // Stolen from mkl46z.h
	// has-a: GpioType
	GpioType mGpioType;
	// has-a: boolean
	bool mIsInitialized;
};

Gpio gGpioA = {PTA_BASE_PTR, kGpioA, false};
Gpio gGpioB = {PTB_BASE_PTR, kGpioB, false};
Gpio gGpioC = {PTC_BASE_PTR, kGpioC, false};
Gpio gGpioD = {PTD_BASE_PTR, kGpioD, false};


Gpio* GpioGet(GpioType inGpioType)
{
	switch (inGpioType)
	{
	case kGpioA:
		return &gGpioA;
		break;
	case kGpioB:
		return &gGpioB;
		break;
	case kGpioC:
		return &gGpioC;
		break;
	case kGpioD:
		return &gGpioD;
		break;
	default:
		//assert(!"Invalid GpioType!");
		break;
	}
	return NULL;
}

// "Constructor" for the gpio
void GpioInitialize(Gpio* ioGpio)
{
	ioGpio->mIsInitialized = true;
}

// "Destructor" for the gpio
void GpioFinalize(Gpio* ioGpio)
{
	ioGpio->mIsInitialized = false;
}

void* GpioGetBaseAddress(const Gpio* inGpio)
{
	//return inGpio->mBaseAddress;
	UNUSED(inGpio);
	return NULL;
}

uint32_t GpioGetRegisterPDDR(const Gpio* inGpio)
{
	const uint32_t value = GPIO_PDDR_REG(inGpio->mBaseAddress);
	return value;
}

void GpioSetRegisterPDDR(Gpio* ioGpio, uint32_t inValue)
{
	GPIO_PDDR_REG(ioGpio->mBaseAddress) = inValue;
}

uint32_t GpioGetRegisterPDOR(const Gpio* inGpio)
{
	const uint32_t value = GPIO_PDOR_REG(inGpio->mBaseAddress);
	return value;
}

void GpioSetRegisterPDOR(Gpio* ioGpio, uint32_t inValue)
{
	GPIO_PDOR_REG(ioGpio->mBaseAddress) = inValue;
}

uint32_t GpioGetRegisterPDIR(const Gpio* inGpio)
{
	const uint32_t value = GPIO_PDIR_REG(inGpio->mBaseAddress);
	return value;
}

void GpioSetRegisterPDIR(Gpio* ioGpio, uint32_t inValue)
{
	GPIO_PDIR_REG(ioGpio->mBaseAddress) = inValue;
}





