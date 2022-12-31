//************************************************************************************************************
// File: simon.h
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef SIMON_H
#define SIMON_H

// nxp
#include "global.h"

// c99
#include <stdbool.h>
#include <string.h>

#define UNUSED(var) (void)(var)

// Constructor-like function that performs all necessary operations to initialize the Simon Game
void SimonInitialize();

// Destructor-like function that performs all necessary operations to finalize the Simon Game
void SimonFinalize();

void GameCycle();

void DisplaySequence();

bool ReadButtons();

bool WasButtonCorrect();

void SimonPlay();

#endif /* SIMON_H */
