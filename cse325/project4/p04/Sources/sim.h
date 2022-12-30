//************************************************************************************************************
// File: sim.h
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef SIM_H_
#define SIM_H_

// c99
#include <stdint.h>

// *Note* Follow "Sim" prefix naming convention to make it explicit 
// that these api's relate to sim functionality.

// *TRICKY* Opaque type hiding details of System Integration Module (SIM) device.
// Prefer to define the "struct Sim" in .c so we can change its contents in the future
struct Sim;
// Reduce typing using typedef
typedef struct Sim Sim;

// Return a pointer to the single global sim instance.
Sim* SimGet();

// Constructor-like function that performs all necessary operations to initialize SIM
void SimInitialize(Sim* ioSim);

// Destructor-like function that performs all necessary operations to finalize SIM
void SimFinalize(Sim* ioSim);

// Return the location of the SCGC5 register
uint32_t SimGetRegisterSCGC5(const Sim* inSim);

// Set the SCGC5 register address to hold the input value
void SimSetRegisterSCGC5(Sim* ioSim, uint32_t inValue);

// Return the location of the SCGC6 register
uint32_t SimGetRegisterSCGC6(const Sim* inSim);

// Set the SCGC6 register address to hold the input value
void SimSetRegisterSCGC6(Sim* ioSim, uint32_t inValue);

uint32_t SimGetRegisterSOPT2(const Sim* inSim);

void SimSetRegisterSOPT2 (Sim* ioSim, uint32_t inValue);

// Return a pointer to the Sim's base address
void* SimGetBaseAddress(const Sim* inSim);

#endif /* SIM_H_ */
