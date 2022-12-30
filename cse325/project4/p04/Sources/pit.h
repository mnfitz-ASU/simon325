//************************************************************************************************************
// File: pit.h
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef PIT_H
#define PIT_H
// nxp
#include "global.h"

// c99
#include <stdbool.h>
#include <string.h>

#define UNUSED(var) (void)(var)

// Structure that defines an instance of the Programmable Interval Timer device

struct Pit;
typedef struct Pit Pit;

enum PitTimer
{
	kPit0,
	kPit1
};
typedef enum PitTimer PitTimer;

// Return a pointer to the single global PIT instance.
Pit* PitGet(PitTimer inTimer);

// Constructor-like function that performs all necessary operations to initialize PIT
void PitInitialize(Pit* ioPit);

// Destructor-like function that performs all necessary operations to finalize PIT
void PitFinalize(Pit* ioPit);

// Function that gets called whenever a PIT timer interrupt occurs
typedef void(*PitTimerCallBack)(Pit* ioPit);

void PitStartTimer(Pit* ioPit, uint32_t inMSec, PitTimerCallBack inCallBack);

void PitStopTimer(Pit* ioPit);

// Return True if the Pit Timer ioPit has expired
// *Note: Reading this value automatically clears it
bool PitGetDidTrigger(Pit* ioPit);

#endif /* PIT_H */
