//************************************************************************************************************
// File: pushb.h
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef PUSHB_H_
#define PUSHB_H_
// nxp
#include "global.h"

// c99
#include <stdbool.h>
#include <string.h>

#define UNUSED(var) (void)(var)

// Structure that defines an instance of a Push Button
struct PushB; 
typedef struct PushB PushB;

enum ButtonColor
{
	kRedButton,
	kOrangeButton,
	kYellowButton,
	kGreenButton
};
typedef enum ButtonColor ButtonColor;

// Return a pointer to a global button instance.
PushB* PushBGet(ButtonColor inColor);

// Constructor-like function that performs all necessary operations to initialize button
void PushBInitialize(PushB* ioPushB);

// Destructor-like function that performs all necessary operations to finalize button
void PushBFinalize(PushB* ioPushB);

bool PushBGetIsPressed(const PushB* inPushB);

void PushBDebouncer(PushB* ioPushB);

#endif /* PUSHB_H_ */
