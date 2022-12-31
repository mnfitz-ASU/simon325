//************************************************************************************************************
// File: tpm.h
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************

#ifndef TPM_H_
#define TPM_H_

// c99
#include <stdint.h>

// *Note* Follow "TPM" prefix naming convention to make it explicit 
// that these api's relate to TPM functionality.

// *TRICKY* Opaque type hiding details of Trusted Platform Module (TPM) device.
// Prefer to define the struct TPM in .c so we can change its contents in the future
struct Tpm;
// Reduce typing using typedef
typedef struct Tpm Tpm;

enum TpmTimer
{
	kTimer0,
	kTimer1,
	kTimer2
};
typedef enum TpmTimer TpmTimer;

// Return a pointer to the TPM instance
Tpm* TpmGet(TpmTimer inTimer);

// Constructor-like function that performs all necessary operations to initialize the TPM
void TpmInitialize(Tpm* ioTpm);

// Destructor-like function that performs all necessary operations to finalize TPM
void TpmFinalize(Tpm* ioTpm);

// Return a pointer to the GPIO device address
void* TpmGetBaseAddress(const Tpm* inTpm);

//Configures a TPM timer to implement a busy delay loop that continuously polls TPM_SC[TOF] to delay for exactly 
//inDelay microseconds, where 1 <= inDelay <= 1000.
void TpmDelayUsec(Tpm* ioTpm, uint32_t inDelayUsec);

//Configures one of the TPM timers to implement a busy delay loop that continuously polls TPM_SC[TOF] to delay for exactly 
//inDelay microseconds, where 1 <= inDelay <= (2^32)- 1.
void TpmDelayMsec(Tpm* ioTpm, uint32_t inDelayMsec);

#endif /* TPM_H_ */
