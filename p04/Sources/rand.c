//************************************************************************************************************
// File: rand.c
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************

// project 4
#include "rand.h"

// nxp
#include "global.h"

// c99
#include <stdlib.h>
//#include <stdbool.h>
//#include <string.h>

#define UNUSED(var) (void)(var)

// Structure that defines an instance of the Randomizer

struct Rand
{
	bool mIsInitialized;
	uint32_t mSeed;
	uint32_t mValue;
};
typedef struct Rand Rand;
/*

There are various techniques used in computers for generating random numbers . Many Î¼p's have a built-in hardware
random number generation module but the MKL46Z does not. Still, there are numerous software-based techniques. The
one we will use in this project is to implement what is called a linear congruential generator (LCG)5 . An LCG generates
random numbers using the formula,
next = (A· prev + C) mod M
which requires a starting value for prev, which is called the seed. Note that the "random" numbers generated using this
formula are, in fact, quite predictable. For any starting seed value, the same sequence of numbers will be generated;
hence, the numbers are not statistically random, and are more correctly called pseudorandom numbers.
There is a well-developed theory for choosing the values of A, C, and M. Without going into the details, we shall choose A
= 1,103,515,245, C = 12345, and M = 231 (these are the values used for the pseudorandom number generator in the
GNU C Library (glibc). Note A, C, M, prev and next will be 32-bit unsigned integers (uint32_t) and all computations are
performed ignoring overflow.

*/

/*

The ARM Cortex-M0+ contains a timer called SysTick which, in the MKL46Z,
is disabled at reset. We can initialize SysTick and then read the current value of the timer to obtain our seed. SysTick
consists of three registers SYST_CSR (SysTick Control and Status Register), SYST_RVR (SysTick Reload Value
Register), and SYST_CVR (SysTick Current Value Register).
The SYST_RVR contains a 24-bit value which is the starting value of the counter. Once enabled, SysTick will begin
counting down until it reaches 0 and will then reload the counter with the value of SYST_RVR. The current value of the
counter can be read by reading SYST_CVR. The Project 4 zip archive contains a module named SysTick which can be
used to configure and use the SysTick timer. I suggest you initialize the timer by calling systick_init() to generate interrupts
at a 200 Hz rate. During the hardware initialization part of your code, after SysTick is up and running, you can call
systick_read() to read the current value of the timer and use that value as your pseudorandom number generator seed.

*/

Rand* RandGet()
{
	static Rand sRand = {false};
	return &sRand;
}

// Constructor-like function that performs all necessary operations to initialize Rand
void RandInitialize(Rand* ioRand, uint32_t inSeed)
{
	ioRand->mSeed = inSeed;
	ioRand->mValue = inSeed;
	ioRand->mIsInitialized = true;
}

// Destructor-like function that performs all necessary operations to finalize Rand
void RandFinalize(Rand* ioRand)
{
	ioRand->mValue = 0;
	ioRand->mSeed = 0;
	ioRand->mIsInitialized = false;
}

void RandInit(uint32_t inSeed)
{
	Rand* random = RandGet();
	RandInitialize(random, inSeed);
}

uint32_t RandNext()
{
	// This is a linear congruential generator (LCG)5 . An LCG generates
	// random numbers using the formula:
	
	//next = (A· prev + C) mod M
	//A = 1,103,515,245, C = 12345, and M = 231
	
	Rand* random = RandGet();
	
	enum 
	{
		kA = 1103515245,
		kC = 12345,
		kM = 231
	};
	random->mValue = (kA * random->mValue + kC) % kM;
	return random->mValue;
}

uint32_t RandBetween(int inLower, int inUpper)
{
	int delta = inUpper - inLower;
	const uint32_t result = (RandNext() % delta) + inLower;
		
	return result;
}

