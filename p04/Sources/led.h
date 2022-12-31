//************************************************************************************************************
// File: led.h
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef LED_H_
#define LED_H_

// c99
#include <stdint.h>

// *Note* Follow "Led" prefix naming convention to make it explicit 
// that these api's relate to LED functionality.

// *TRICKY* Opaque type hiding details of Light Emitting Diode (LED) device.
// Prefer to define the struct Led in .c so we can change its contents in the future
struct Led;
// Reduce typing using typedef
typedef struct Led Led;

enum LedColor
{
	kRed,
	kOrange,
	kYellow,
	kGreen,
	kInternal
};
// Reduce typing using typedef
typedef enum LedColor LedColor;

// Return a pointer to the LED instance that has that color
Led* LedGet(LedColor inColor);

// Constructor-like function that performs all necessary operations to initialize the led and set it to off
void LedInitialize(Led* ioLed);

// Destructor-like function that performs all necessary operations to finalize led
void LedFinalize(Led* ioLed);

void LedOn(Led* ioLed);

void LedOff(Led* ioLed);

void LedToggle(Led* ioLed);

void LedBitDisplay(int inBits, Led* ioBit0, Led* ioBit1, Led* ioBit2, Led* ioBit3);

#endif /* LED_H_ */
