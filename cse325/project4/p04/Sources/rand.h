//************************************************************************************************************
// File: rand.h
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef RAND_H
#define RAND_H
// nxp
#include "global.h"

// c99
#include <stdbool.h>
#include <string.h>

#define UNUSED(var) (void)(var)

// Structure that defines an instance of the Randomizer

void RandInit(uint32_t inSeed);

uint32_t RandNext();

uint32_t RandBetween(int inLower, int inUpper);

#endif /* RAND_H */
