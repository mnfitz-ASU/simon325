//************************************************************************************************************
// File: pit.c
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************

// project 4
#include "int.h"
#include "pit.h"
#include "sim.h"

// nxp
#include "global.h"

// c99
#include <stdbool.h>
#include <string.h>

#define UNUSED(var) (void)(var)

// Structure that defines an instance of the Programmable Interval Timer device

uint32_t PitGetRegisterMCR(const Pit* inPort);

void PitSetRegisterMCR(Pit* ioPort, uint32_t inValue);

uint32_t PitGetRegisterTCtrl(const Pit* inPort);

void PitSetRegisterTCtrl(Pit* ioPort, uint32_t inValue);

uint32_t PitGetRegisterLDVal(const Pit* inPort);

void PitSetRegisterLDVal(Pit* ioPort, uint32_t inValue);

uint32_t PitGetRegisterTFLG(const Pit* inPort);

void PitSetRegisterTFLG(Pit* ioPort, uint32_t inValue);

struct Pit
{
	//void* mBaseAddress;
	PIT_MemMapPtr mBaseAddress; // Stolen from mkl46z.h
	int mTimerNumber; 
	bool mIsInitialized;
	PitTimerCallBack mCallback;
	bool mIsStarted;
	bool mDidTrigger;
	uint32_t mCountdown;
};

/*
 32.5 Initialization and application information
In the example configuration:
• The PIT clock has a frequency of 50 MHz.
• Timer 1 creates an interrupt every 5.12 ms.
• Timer 3 creates a trigger event every 30 ms.
The PIT module must be activated by writing a 0 to MCR[MDIS].
The 50 MHz clock frequency equates to a clock period of 20 ns. Timer 1 needs to trigger
every 5.12 ms/20 ns = 256,000 cycles and Timer 3 every 30 ms/20 ns = 1,500,000 cycles.
The value for the LDVAL register trigger is calculated as:
LDVAL trigger = (period / clock period) -1
This means LDVAL1 and LDVAL3 must be written with 0x0003E7FF and 0x0016E35F
respectively.
The interrupt for Timer 1 is enabled by setting TCTRL1[TIE]. The timer is started by
writing 1 to TCTRL1[TEN].
Timer 3 shall be used only for triggering. Therefore, Timer 3 is started by writing a 1 to
TCTRL3[TEN]. TCTRL3[TIE] stays at 0.

MCR[MDIS]
LDVAL
TCTRLn[TIE][TEN]
TFLG

// turn on PIT
 PIT_MCR = 0x00;
 // Timer 1
 PIT_LDVAL1 = 0x0003E7FF; // setup timer 1 for 256000 cycles
 PIT_TCTRL1 = TIE; // enable Timer 1 interrupts 
 PIT_TCTRL1 |= TEN; // start Timer 1
 // Timer 3
 PIT_LDVAL3 = 0x0016E35F; // setup timer 3 for 1500000 cycles
 PIT_TCTRL3 |= TEN; // start Timer 3
 

#define PIT_MCR_REG(base)                        ((base)->MCR)
#define PIT_LTMR64H_REG(base)                    ((base)->LTMR64H)
#define PIT_LTMR64L_REG(base)                    ((base)->LTMR64L)
#define PIT_LDVAL_REG(base,index)                ((base)->CHANNEL[index].LDVAL)
#define PIT_CVAL_REG(base,index)                 ((base)->CHANNEL[index].CVAL)
#define PIT_TCTRL_REG(base,index)                ((base)->CHANNEL[index].TCTRL)
#define PIT_TFLG_REG(base,index)                 ((base)->CHANNEL[index].TFLG)
 */

// Return a pointer to the single global PIT instance.

//----------------------------------------------------------------------------------------------------------------------
// FUNCTION: pit_isr()
//
// DESCRIPTION
// The PIT ISR which overrides the default pit_isr() function declared in bare_startup.c.
//
// NOTE
// The PIT interrupt request flag is automatically cleared.
//----------------------------------------------------------------------------------------------------------------------
void pit_isr()
{
	// Isr runs at interrupt level. Must do as little work possible here!!!
	// At this point, we know that 1 or more timers have caused an interrupt.
	// First, check each timer if it has expired, and if so, invoke any registered callback.
	const uint32_t kTimerExpired = 0x1;
	
	Pit* pit0 = PitGet(kPit0);
	const uint32_t kTimer0Masked = PitGetRegisterTFLG(pit0) & kTimerExpired;

	const bool isTimer0Expired = (kTimer0Masked == kTimerExpired);
	if (isTimer0Expired) // Check if timer0 is expired
	{
		if (pit0->mCallback != NULL) 
		{
			pit0->mCallback(pit0);
		}
		PitSetRegisterTFLG(pit0, kTimerExpired);
		pit0->mDidTrigger = true;
	}
	
	Pit* pit1 = PitGet(kPit1);
	const uint32_t kTimer1Masked = PitGetRegisterTFLG(pit1) & kTimerExpired;

	const bool isTimer1Expired = (kTimer1Masked == kTimerExpired);
	if (isTimer1Expired) // Check if timer1 is expired
	{
		if (pit1->mCallback != NULL)
		{
			pit1->mCallback(pit1);
		}
		PitSetRegisterTFLG(pit1, kTimerExpired);
		pit1->mDidTrigger = true;
	}
}

void PitIRQEnable() 
{
	// IRQ 22
	uint32_t nvic_ipr = NVIC_IPR5; 
	nvic_ipr &= ~(0b11111111 << 16); // Clear PRI_22 field
	nvic_ipr |= 0b01000000 << 16; // Write 0b01 (priority 1) to bits 7:6 of PRI_22; //write 0's to bits 5:0
	NVIC_IPR5 = nvic_ipr; // Update NVIC_IPR register
	NVIC_ISER |= 1 << 22;
}

void PitOneTimeInitialization()
{
	//SIM_SCGC6 |= (0b1<<23);
	Sim* sim = SimGet();
	uint32_t scgc6 = SimGetRegisterSCGC6(sim);
	const int kSCGC6Enable = (0b1 << 23);
	SimSetRegisterSCGC6(sim, (scgc6 | kSCGC6Enable));
	
	// Enable Interrupt to take interrupts from pit at priority level 1.
	int_init(int_src_pit, int_priority_1, int_state_enabled);	
	
	PitIRQEnable();
}

Pit* PitGet(PitTimer inTimer)
{
	static bool sIsOneTimeInitialized = false; // Tricky: Static = variable is persistent; effectively a global hidden in this function
	if (!sIsOneTimeInitialized)
	{
		PitOneTimeInitialization();
		sIsOneTimeInitialized = true;
	}
	
	// mBaseAddress, mTimerNumber, mIsInitialized, mCallback, mIsStarted, mDidTrigger, mCountdown;
	static Pit sPit0 = {PIT_BASE_PTR, 0, false, NULL, false, false, 0};
	static Pit sPit1 = {PIT_BASE_PTR, 1, false, NULL, false, false, 0};
	
	switch(inTimer)
	{
	case kPit0:
		return &sPit0;
		break;
	case kPit1:
		return &sPit1;
		break;
	default:
		// How did we get here?
		//assert(!"Invalid PitTimer enum value!")
		break;
	}
	
	return NULL;
}

// Constructor-like function that performs all necessary operations to initialize PIT
void PitInitialize(Pit* ioPit)
{	
	//PIT_MCR = 0x00;
	uint32_t kPitClockEnable = 0x00;
	PitSetRegisterMCR(ioPit, kPitClockEnable);
	
	ioPit->mIsInitialized = true;
}
/*
void PitIRQDisable() 
{
	// IRQ 22
	uint32_t nvic_ipr = NVIC_IPR5; 
	nvic_ipr &= ~(0b11111111 << 16); // Clear PRI_22 field
	nvic_ipr |= 0b01000000 << 16; // Write 0b01 (priority 1) to bits 7:6 of PRI_22; //write 0's to bits 5:0
	NVIC_IPR5 = nvic_ipr; // Update NVIC_IPR register
	NVIC_ISER |= 1 << 22;
}
*/

// Destructor-like function that performs all necessary operations to finalize PIT
void PitFinalize(Pit* ioPit)
{
	uint32_t kPitClockDisable = 0x10;
	PitSetRegisterMCR(ioPit, kPitClockDisable);
	
	//PIT_TCTRL0 = 0x3; // enable Timer 0 interrupts and start timer.
	uint32_t kDisableTimer = 0x0;
	PitSetRegisterTCtrl(ioPit, kDisableTimer);
	
	//PitIRQDisable();
		
	ioPit->mIsInitialized = false;
}

void PitStartTimer(Pit* ioPit, uint32_t inMSec, PitTimerCallBack inCallBack)
{
	//PIT_MCR = 0x00;
	uint32_t kPitClockEnable = 0x00;
	PitSetRegisterMCR(ioPit, kPitClockEnable);
	
	//PIT_LDVAL1 = countdownValue; // setup timer 
	int kToNSec = 1000000;
	int kClockTickInNSec = 20; // 50 MHz, the ClockTick in nanoseconds 
	uint32_t countdownValue = ((inMSec*kToNSec) / kClockTickInNSec) -1; 
	PitSetRegisterLDVal(ioPit, countdownValue);
	
	ioPit->mCallback = inCallBack;
		
	//PIT_TCTRL1 = TIE | TEN; // enable Timer and Timer Interrupts 
	const uint32_t kEnableTimer = 0b1;
	const uint32_t kEnableInterrupt = 0b10;
	const uint32_t kEnable = (kEnableTimer | kEnableInterrupt);

	const uint32_t tCtrl = PitGetRegisterTCtrl(ioPit) | kEnable;
	PitSetRegisterTCtrl(ioPit, tCtrl);
	
	ioPit->mIsStarted = true;
	ioPit->mDidTrigger = false;
	ioPit->mCountdown = countdownValue;
}

void PitStopTimer(Pit* ioPit)
{	
	ioPit->mCallback = NULL;
		
	//PIT_TCTRL1 = TIE | TEN; // enable Timer and Timer Interrupts 
	const uint32_t kEnableTimer = 0b1;
	const uint32_t kEnableInterrupt = 0b10;
	const uint32_t kDisable = ~(kEnableTimer | kEnableInterrupt);

	const uint32_t tCtrl = PitGetRegisterTCtrl(ioPit) & kDisable;
	PitSetRegisterTCtrl(ioPit, tCtrl);
	
	ioPit->mCountdown = 0;
	ioPit->mDidTrigger = false;
	ioPit->mIsStarted = false;
}

bool PitGetDidTrigger(Pit* ioPit)
{
	// Tricky: Read + Modify of mDidTrigger. Disable interrupts during this operation
	int_mask_all();
	//{
	const bool didTrigger = ioPit->mDidTrigger;
	ioPit->mDidTrigger = false;
	//}
	int_unmask_all();
	return didTrigger;
}

uint32_t PitGetRegisterMCR(const Pit* inPit)
{
	const uint32_t mcr = PIT_MCR_REG(inPit->mBaseAddress); 
	return mcr;
}

void PitSetRegisterMCR(Pit* ioPit, uint32_t inValue)
{
	PIT_MCR_REG(ioPit->mBaseAddress) = inValue;
}

uint32_t PitGetRegisterTCtrl(const Pit* inPit)
{
	const uint32_t tCtrl = PIT_TCTRL_REG(inPit->mBaseAddress, inPit->mTimerNumber); 
	return tCtrl;
}

void PitSetRegisterTCtrl(Pit* ioPit, uint32_t inValue)
{
	PIT_TCTRL_REG(ioPit->mBaseAddress, ioPit->mTimerNumber) = inValue;
}

uint32_t PitGetRegisterLDVal(const Pit* inPit)
{
	const uint32_t lDVal = PIT_LDVAL_REG(inPit->mBaseAddress, inPit->mTimerNumber); 
	return lDVal;
}

void PitSetRegisterLDVal(Pit* ioPit, uint32_t inValue)
{
	PIT_LDVAL_REG(ioPit->mBaseAddress, ioPit->mTimerNumber) = inValue;
}

uint32_t PitGetRegisterTFLG(const Pit* inPit)
{
	const uint32_t tFLG = PIT_TFLG_REG(inPit->mBaseAddress, inPit->mTimerNumber); 
	return tFLG;
}

void PitSetRegisterTFLG(Pit* ioPit, uint32_t inValue)
{
	PIT_TFLG_REG(ioPit->mBaseAddress, ioPit->mTimerNumber) = inValue;
}


