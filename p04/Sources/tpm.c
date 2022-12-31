//************************************************************************************************************
// File: tpm.c
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************

#include "tpm.h"
#include "sim.h"

// nxp
#include "global.h"

// c99
#include <stdbool.h>
#include <string.h>

#define UNUSED(var) (void)(var)
// Base Ptr: TPM0_BASE_PTR

// Return the location of the TPM's SC register
//uint32_t TpmGetRegisterSC(const Tpm* inTpm);

// Set the TPM's SC register address to hold the input value
//void TpmSetRegisterSC(Tpm* ioTpm, uint32_t inValue);

enum TpmCMOD
{
	kCMODDisable = (0b00 << 3),
	kCMODIncrement1 = (0b01 << 3),
	kCMODIncrement2 = (0b10 << 3),
	
	kCMODMask = (0b11 << 3)
};
typedef enum TpmCMOD TpmCMOD;

void TpmSetRegisterSC_CMOD(Tpm* ioTpm, TpmCMOD inValue);

TpmCMOD TpmGetRegisterSC_CMOD(Tpm* ioTpm);

enum TpmPS
{
	kPSDiv1   = (0b000),
	kPSDiv2   = (0b001),
	kPSDiv4   = (0b010),
	kPSDiv8   = (0b011),
	kPSDiv16  = (0b100),
	kPSDiv32  = (0b101),
	kPSDiv64  = (0b110),
	kPSDiv128 = (0b111),
	
	kPSMask   = (0b111)
};
typedef enum TpmPS TpmPS;

void TpmSetRegisterSC_PS(Tpm* ioTpm, TpmPS inValue);

TpmPS TpmGetRegisterSC_PS(Tpm* ioTpm);

void TpmSetRegisterSC_TOF(Tpm* ioTpm, bool inValue);

bool TpmGetRegisterSC_TOF(Tpm* ioTpm);

// Return the location of the TPM's Mod register
uint32_t TpmGetRegisterMod(const Tpm* inTpm);

// Set the TPM's Count register address to hold the input value
void TpmSetRegisterCnt(Tpm* ioTpm, uint32_t inValue); 

// Set the TPM's Mod register address to hold the input value
void TpmSetRegisterMod(Tpm* ioTpm, uint32_t inValue);


struct Tpm
{
	// has-a: base address
	TPM_MemMapPtr mBaseAddress; // Stolen from mkl46z.h
	// has-a: TpmTimer
	TpmTimer mTimer;
	// has-a: enable bit
	uint32_t mEnableBit;
	// has-a: source frequency
	uint32_t mSrcFreqHz;
	// has-a: boolean
	bool mIsInitialized;
};

// TpmName = {baseAddress, timerNumber, enableBit, srcFreq, isInitialized};

Tpm gTpm0 = {TPM0_BASE_PTR, kTimer0, (1<<24), 0, false};
Tpm gTpm1 = {TPM1_BASE_PTR, kTimer1, (1<<25), 0, false};
Tpm gTpm2 = {TPM2_BASE_PTR, kTimer2, (1<<26), 0, false};

uint32_t gTpmSrcFreqHz = 0;

struct TpmDelay
{
	// has-a: TpmPs
	TpmPS mTpmPS;
	// has-a: PreScalar
	uint32_t mDivisor;
	// has-a: Max time
	float mMaxDelayMsec;
};
typedef struct TpmDelay TpmDelay;

// TpmDelay gPSDivExample = {TpmPs, mDivisor, mMaxDelayMsec};
TpmDelay gPSDiv1   = {kPSDiv1, 1, 0 /*8.192f*/};	   // mMaxDelay = ((2^16)-1)+1)/(gTpmSrcFreqHz / PreScalar (1))
TpmDelay gPSDiv2   = {kPSDiv2, 2, 0 /*16.384f*/};      // mMaxDelay = ((2^16)-1)+1)/(gTpmSrcFreqHz / PreScalar (2))
TpmDelay gPSDiv4   = {kPSDiv4, 4, 0 /*32.768f*/};      // mMaxDelay = ((2^16)-1)+1)/(gTpmSrcFreqHz / PreScalar (4))
TpmDelay gPSDiv8   = {kPSDiv8, 8, 0 /*65.536f*/};      // mMaxDelay = ((2^16)-1)+1)/(gTpmSrcFreqHz / PreScalar (8))
TpmDelay gPSDiv16  = {kPSDiv16, 16, 0 /*131.072f*/};   // mMaxDelay = ((2^16)-1)+1)/(gTpmSrcFreqHz / PreScalar (16))
TpmDelay gPSDiv32  = {kPSDiv32, 32, 0 /*262.144f*/};   // mMaxDelay = ((2^16)-1)+1)/(gTpmSrcFreqHz / PreScalar (32))
TpmDelay gPSDiv64  = {kPSDiv64, 64, 0 /*524.288f*/};   // mMaxDelay = ((2^16)-1)+1)/(gTpmSrcFreqHz / PreScalar (64))
TpmDelay gPSDiv128 = {kPSDiv128, 128, 0 /*1048.576f*/};// mMaxDelay = ((2^16)-1)+1)/(gTpmSrcFreqHz / PreScalar (128))

void TpmOneTimeInitialization()
{
	enum TpmPllFll
	{
		// Selects the MCGPLLCLK or MCGFLLCLK clock for various peripheral clocking options.
		kMCGFLL = (0b0 << 16),
		kMCGPLL = (0b1 << 16),
		
		kPllFllMask = (kMCGFLL | kMCGPLL)
	};
	
	enum TpmSrc
	{
		// Selects the clock source for the TPM counter clock
		kSrcDisable = (0b00 << 24), // Disabled
		kSrcMCGFLL  = (0b01 << 24), // System clock with other set logic
		kSrcOSCER   = (0b10 << 24), // 8 MhZ oscillator clock
		kSrcMCGIR   = (0b11 << 24), // 4MhZ or 32KhZ system clock
		
		kSrcMask    = (kSrcDisable | kSrcMCGFLL | kSrcOSCER | kSrcMCGIR)
	};
	
	Sim* sim = SimGet();
	
	uint32_t sopt2 = SimGetRegisterSOPT2(sim);
	sopt2 &= ~(kPllFllMask | kSrcMask);
	sopt2 |= (kMCGFLL | kSrcOSCER); // !DANGER! : Setting this clock means you must put 8M in the global below! 
	SimSetRegisterSOPT2(sim, sopt2);
	
	gTpmSrcFreqHz = 8000000; // kSrcOSCER = 8 MhZ
	
	const float kToMsec = 1000.0f;
	// We can't assume order of initialization for globals, so we enforce the correct ordering at runtime.
	gPSDiv1.mMaxDelayMsec = ((1<<16)/(gTpmSrcFreqHz / 1.0f)) * kToMsec;
	gPSDiv2.mMaxDelayMsec = ((1<<16)/(gTpmSrcFreqHz / 2.0f)) * kToMsec;
	gPSDiv4.mMaxDelayMsec = ((1<<16)/(gTpmSrcFreqHz / 4.0f)) * kToMsec;
	gPSDiv8.mMaxDelayMsec = ((1<<16)/(gTpmSrcFreqHz / 8.0f)) * kToMsec;
	gPSDiv16.mMaxDelayMsec = ((1<<16)/(gTpmSrcFreqHz / 16.0f)) * kToMsec;
	gPSDiv32.mMaxDelayMsec = ((1<<16)/(gTpmSrcFreqHz / 32.0f)) * kToMsec;
	gPSDiv64.mMaxDelayMsec = ((1<<16)/(gTpmSrcFreqHz / 64.0f) * kToMsec);
	gPSDiv128.mMaxDelayMsec = ((1<<16)/(gTpmSrcFreqHz / 128.0f)) * kToMsec;
}

// Return a pointer to the TPM instance
Tpm* TpmGet(TpmTimer inTimer)
{
	static bool sIsOneTimeInitialized = false; // Tricky: Static = variable is persistent; effectively a global hidden in this function
	if (!sIsOneTimeInitialized)
	{
		TpmOneTimeInitialization();
		sIsOneTimeInitialized = true;
	}
	
	switch (inTimer)
	{
	case kTimer0:
		return &gTpm0;
	case kTimer1:
		return &gTpm1;
	case kTimer2:
		return &gTpm2;
	default:
		break;
	}
	// Check out my tricky way of using assert(false)!
	//assert(!"What kind of timer was this?");	// How did we get here?
	return NULL; // Only happens after a "default" switch
}

// Constructor-like function that performs all necessary operations to initialize the TPM
void TpmInitialize(Tpm* ioTpm)
{
	// Enable SCGC6
	//SIM_SCGC6() |= ioTpm->mEnableBit;
	Sim* sim = SimGet();
	const uint32_t scgc6 = SimGetRegisterSCGC6(sim) ;
	SimSetRegisterSCGC6(sim, scgc6 | ioTpm->mEnableBit);
	
	ioTpm->mSrcFreqHz = gTpmSrcFreqHz;
	ioTpm->mIsInitialized = true;
}

// Destructor-like function that performs all necessary operations to finalize TPM
void TpmFinalize(Tpm* ioTpm)
{
	// Disable timer is SCGC6
	//SIM_SCGC6() |= ioTpm->mEnableBit;
	Sim* sim = SimGet();
	const uint32_t scgc6 = SimGetRegisterSCGC6(sim) ;
	SimSetRegisterSCGC6(sim, scgc6 & ~(ioTpm->mEnableBit));
	ioTpm->mIsInitialized = false;
}

// Return a pointer to the GPIO device address
void* TpmGetBaseAddress(const Tpm* inTpm)
{
	UNUSED(inTpm);
	return NULL;
}

// We assume that for Project 4, we will always use TPM0 for delay

//Configures a TPM timer to implement a busy delay loop that continuously polls TPM_SC[TOF] to delay for exactly 
//inDelay microseconds, where 1 <= inDelayUsec <= 1000.
void TpmDelayUsec(Tpm* ioTpm, uint32_t inDelayUsec)
{
	//assert(inDelayMsec <= 1000);
	
	// Set Pre-scalar to be 1
	// Calculate mod by multiplying inDelay by mSrcFreqHz, then subtracting 1 by that
	// Timer Period = (MOD + 1) / (8000000 / 1)
	
	// We must disable timer to make changes to it
	TpmSetRegisterSC_CMOD(ioTpm, kCMODDisable);

	// Set the pre-scalar
	TpmSetRegisterSC_PS(ioTpm, kPSDiv1); //  Max value we can count up to using 16 bit counter @ 8MhZ = .008191875 seconds, or ~ 8 milliseconds
	
	// Write a zero to the counter
	TpmSetRegisterCnt(ioTpm, 0); 
	
	//Use single precision, and only double precision when necessary
	const float kToUsec = 1/1000000.0f;
	float scaler = ((ioTpm->mSrcFreqHz / 1)-1) * kToUsec;
	
	// Set the modulo field
	int modulo = (int)((inDelayUsec * scaler)-1); // Calculate value from equation
	TpmSetRegisterMod(ioTpm, modulo); 
	
	// Start timer
	TpmSetRegisterSC_CMOD(ioTpm, kCMODIncrement1);
	
	// Check if there is an expiration
	while (!TpmGetRegisterSC_TOF(ioTpm))
	{
		UNUSED(ioTpm);
		// Wait until Overflow Flag
	} 
	
	// Disable timer since we aren't using it anymore
	TpmSetRegisterSC_CMOD(ioTpm, kCMODDisable);

}



TpmDelay* TpmDelayGet(TpmPS inTpmPS)
{
	switch (inTpmPS)
	{
	case kPSDiv1:
		return &gPSDiv1;
	case kPSDiv2:
		return &gPSDiv2;
	case kPSDiv4:
		return &gPSDiv4;
	case kPSDiv8:
		return &gPSDiv8;
	case kPSDiv16:
		return &gPSDiv16;
	case kPSDiv32:
		return &gPSDiv32;
	case kPSDiv64:
		return &gPSDiv64;
	case kPSDiv128:
		return &gPSDiv128;
	default:
		break;
	}
	// Check out my tricky way of using assert(false)!
	//assert(!"What kind of TpmPS was this?");	// How did we get here?
	return NULL; // Only happens after a "default" switch
}

TpmDelay* MsecToTpmDelay(float inMsec)
{
	TpmDelay* tpmDelay = TpmDelayGet(kPSDiv1);
	if (inMsec < tpmDelay->mMaxDelayMsec)
		return tpmDelay;

	tpmDelay = TpmDelayGet(kPSDiv2);
	if (inMsec < tpmDelay->mMaxDelayMsec)
		return tpmDelay;
	
	tpmDelay = TpmDelayGet(kPSDiv4);
	if (inMsec < tpmDelay->mMaxDelayMsec)
		return tpmDelay;
	
	tpmDelay = TpmDelayGet(kPSDiv8);
	if (inMsec < tpmDelay->mMaxDelayMsec)
		return tpmDelay;

	tpmDelay = TpmDelayGet(kPSDiv16);
	if (inMsec < tpmDelay->mMaxDelayMsec)
		return tpmDelay;

	tpmDelay = TpmDelayGet(kPSDiv32);
	if (inMsec < tpmDelay->mMaxDelayMsec)
		return tpmDelay;
	
	tpmDelay = TpmDelayGet(kPSDiv64);
	if (inMsec < tpmDelay->mMaxDelayMsec)
		return tpmDelay;

	tpmDelay = TpmDelayGet(kPSDiv128);
	return tpmDelay;
}

void TpmDelayMsec(Tpm* ioTpm, uint32_t inDelayMsec)
{ 
	TpmSetRegisterSC_CMOD(ioTpm, kCMODDisable);

	// We use float since the time elapsed is measured in fractional milliseconds
	float delayMsec = inDelayMsec; 
	while (delayMsec > 0)
	{
		// Timer Period(in Usec) = (MOD + 1) / (8000000 / PreScalar)
		TpmDelay* tpmDelay = MsecToTpmDelay(delayMsec);
		TpmSetRegisterSC_PS(ioTpm, tpmDelay->mTpmPS); //  Max value we can count up to using 16 bit counter @ 8MhZ = .008191875 seconds, or ~ 8 milliseconds
		
		// Write a zero to the counter
		TpmSetRegisterCnt(ioTpm, 0); 

		//Use single precision, and only double precision when necessary
		const float kToMsec = 1/1000.0f;
		const float scalerValue = ((ioTpm->mSrcFreqHz / tpmDelay->mDivisor)-1) * kToMsec;

		// Set the modulo field
		const bool needMaxMod = (tpmDelay->mMaxDelayMsec < delayMsec);
		if (needMaxMod)
		{
			const uint32_t modulo = (1 << 16)-1; // Maximum value a 16 bit number can be (65535)
			TpmSetRegisterMod(ioTpm, modulo); 
		}
		else
		{
			const uint32_t modulo = (uint32_t)((delayMsec * scalerValue)-1); // Calculate value from equation
			TpmSetRegisterMod(ioTpm, modulo); 
		}
	
		// Start timer
		TpmSetRegisterSC_CMOD(ioTpm, kCMODIncrement1);

		// Check if there is an expiration
		while (!TpmGetRegisterSC_TOF(ioTpm))
		{
			UNUSED(ioTpm);
			// Wait until Overflow Flag
		} 
		// The TOF bit is cleared by writing a 1 to TOF bit. Writing a 0 to TOF has no effect
		TpmSetRegisterSC_TOF(ioTpm, true);

		// Update Time Remaining
		delayMsec -= tpmDelay->mMaxDelayMsec; 
	}
	// Disable timer since we aren't using it anymore
	TpmSetRegisterSC_CMOD(ioTpm, kCMODDisable);
}

// Return the location of the TPM's SC register
uint32_t TpmGetRegisterSC(const Tpm* inTpm)
{
	const uint32_t value = TPM_SC_REG(inTpm->mBaseAddress);
	return value;
}

// Set the TPM's SC register address to hold the input value
void TpmSetRegisterSC(Tpm* ioTpm, uint32_t inValue)
{
	TPM_SC_REG(ioTpm->mBaseAddress) = inValue;
}

// Return the location of the TPM's Mod register
uint32_t TpmGetRegisterMod(const Tpm* inTpm)
{
	const uint32_t value = TPM_MOD_REG(inTpm->mBaseAddress);
	return value;
}

// Set the TPM's Count register address to hold the input value
void TpmSetRegisterCnt(Tpm* ioTpm, uint32_t inValue)
{
	TPM_CNT_REG(ioTpm->mBaseAddress) = inValue;
}

// Set the TPM's Mod register address to hold the input value
void TpmSetRegisterMod(Tpm* ioTpm, uint32_t inValue)
{
	TPM_MOD_REG(ioTpm->mBaseAddress) = inValue;
}


// Helper Functions


void TpmSetRegisterSC_CMOD(Tpm* ioTpm, TpmCMOD inValue)
{
	uint32_t sc = TpmGetRegisterSC(ioTpm);
	sc = (sc & ~kCMODMask) | inValue;
	TpmSetRegisterSC(ioTpm, sc);
}

TpmCMOD TpmGetRegisterSC_CMOD(Tpm* ioTpm)
{
	uint32_t sc = TpmGetRegisterSC(ioTpm);
	sc &= kCMODMask;
	return (TpmCMOD)sc; // DANGER! we assume that sc can be cast to the TpmCMOD enum type.
}

void TpmSetRegisterSC_PS(Tpm* ioTpm, TpmPS inValue)
{
	uint32_t sc = TpmGetRegisterSC(ioTpm);
	sc = (sc & ~kPSMask) | inValue;
	TpmSetRegisterSC(ioTpm, sc);
}

TpmPS TpmGetRegisterSC_PS(Tpm* ioTpm)
{
	uint32_t sc = TpmGetRegisterSC(ioTpm);
	sc &= kPSMask;
	return (TpmPS)sc; // DANGER! we assume that sc can be cast to the TpmPS enum type.
}

void TpmSetRegisterSC_TOF(Tpm* ioTpm, bool inValue)
{
	uint32_t sc = TpmGetRegisterSC(ioTpm);
	sc = (sc & ~(1 << 7)) | ((inValue & 1) << 7); // Tricky: Use a boolean value as a bit
	TpmSetRegisterSC(ioTpm, sc);
}

bool TpmGetRegisterSC_TOF(Tpm* ioTpm)
{
	uint32_t sc = TpmGetRegisterSC(ioTpm);
	sc &= (1 << 7);
	return (sc == 0 ? false : true); // Tricky! We use the ternary operator "?" to condense an if statement!
}

