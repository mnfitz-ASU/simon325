//************************************************************************************************************
// File: led.c
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************

#include "led.h"

// project 2
#include "gpio.h"
#include "port.h"

// nxp
#include "global.h"

// c99
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define UNUSED(var) (void)(var)

// Copied the GPIO value from the impl in main.c
enum PortDMux
{
	kGpio = 1
};

struct Led
{
	// has-a: color
	LedColor mColor;
	// has-a: port type(B/D)
	PortType mPortType;
	// has-a: pin number
	int mPinNumber;
	// has-a: gpio type
	GpioType mGpioType;
	// has-a: on/off state
	bool mIsOn;
	// has-a: initialize boolean
	bool mIsIntialized;
};
/*

35 PTA1 J1:2 SW1 terminal 2 to J1:2 SW1 terminal 3 to GND
36 PTA2 J1:4 SW2 terminal 2 to J1:4 SW2 terminal 3 to GND
96 PTD3 J1:6 SW3 terminal 2 to J1:6 SW3 terminal 3 to GND
42 PTA12 J1:8 SW4 terminal 2 to J1:8 SW4 terminal 3 to GND


*/
// LedName = {kColor, port, pin, isOn, isInitialized};
Led gRedLed = {kRed, kPortB, 0, kGpioB, false, false}; // Connected to J4 Pin 2
Led gOrangeLed = {kOrange, kPortB, 1, kGpioB, false, false}; // Connected to J4 Pin 4
Led gYellowLed = {kYellow, kPortB, 2, kGpioB, false, false}; // Connected to J4 Pin 8
Led gGreenLed = {kGreen, kPortB, 3, kGpioB, false, false}; // Connected to J4 Pin 6

Led gInternalLed = {kInternal, kPortD, 5, kGpioD, false, false};

void* LedGetBaseAddress(const Led* inLed)
{
	//return inLed->mBaseAddress;
	UNUSED(inLed);
	return NULL;
}

Led* LedGet(LedColor inColor)
{
	switch (inColor)
	{
	case kRed:
		return &gRedLed;
	case kOrange:
		return &gOrangeLed;
	case kYellow:
		return &gYellowLed;
	case kGreen:
		return &gGreenLed;
	case kInternal:
		return &gInternalLed;
	default:
		break;
	}
	// Check out my tricky way of using assert(false)!
	//assert(!"What kind of color was this?");	// How did we get here?
	return NULL; // Only happens after a "default" switch
}

void LedInitialize(Led* ioLed)
{
	// Configure Port B pin 5 for the GPIO function by writing 1 the MUX field of PORTD_PCR5.
	const uint32_t kMuxGpio = PORT_PCR_MUX(kGpio);
	const int pin = ioLed->mPinNumber;
	PortSetRegisterPCR(PortGet(ioLed->mPortType), pin, kMuxGpio);
	// Configure a Port D pin as an output pin by writing 1 to the "led pin" in GPIOD_PDDR.
	const uint32_t outputPin = (1 << ioLed->mPinNumber);
	// pddr |= outputPin;
	const uint32_t pddr = GpioGetRegisterPDDR(GpioGet(ioLed->mGpioType));
	GpioSetRegisterPDDR(GpioGet(ioLed->mGpioType), (pddr | outputPin));
	
	ioLed->mIsIntialized = true;
	
	LedOff(ioLed);
}

void LedFinalize(Led* ioLed)
{
	ioLed->mIsIntialized = false;
}

// I think the setting and clearing for On/Off is the opposite of what was documented in the original main.c
void LedOff(Led* ioLed)
{
	//assert(ioLed->mIsInitialized); // We shouldn't be turning on an un-initialized led!
	uint32_t pdor = GpioGetRegisterPDOR(GpioGet(ioLed->mGpioType));
	pdor |= (1 << ioLed->mPinNumber);
	GpioSetRegisterPDOR(GpioGet(ioLed->mGpioType), pdor);
	ioLed->mIsOn = false;
}

void LedOn(Led* ioLed)
{
	//assert(ioLed->mIsInitialized); // We shouldn't be turning off an un-initialized led!
	uint32_t pdor = GpioGetRegisterPDOR(GpioGet(ioLed->mGpioType));
	pdor &= ~(1 << ioLed->mPinNumber);
	GpioSetRegisterPDOR(GpioGet(ioLed->mGpioType), pdor);
	ioLed->mIsOn = true;
}

void LedToggle(Led* ioLed)
{
	if (ioLed->mIsOn)
	{
		LedOff(ioLed);
	}
	else
	{
		LedOn(ioLed);
	}
}

void LedBitDisplay(int inBits, Led* ioBit0, Led* ioBit1, Led* ioBit2, Led* ioBit3)
{
	inBits &= 0xF; // Only read last 4 Bits, 0xF == (0x1 | 0x2 | 0x4 | 0x8)
	const bool isBit3Set = ((inBits & 0x8) != 0x0);
	if (isBit3Set)
	{
		LedOn(ioBit3);
	}
	else
	{
		LedOff(ioBit3);
	}
	
	const bool isBit2Set = ((inBits & 0x4) != 0x0);
	if (isBit2Set)
	{
		LedOn(ioBit2);
	}
	else
	{
		LedOff(ioBit2);
	}
	
	const bool isBit1Set = ((inBits & 0x2) != 0x0);
	if (isBit1Set)
	{
		LedOn(ioBit1);
	}
	else
	{
		LedOff(ioBit1);
	}
	
	const bool isBit0Set = ((inBits & 0x1) != 0x0);
	if (isBit0Set)
	{
		LedOn(ioBit0);
	}
	else
	{
		LedOff(ioBit0);
	}
}




