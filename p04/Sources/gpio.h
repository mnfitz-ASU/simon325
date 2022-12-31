//************************************************************************************************************
// File: gpio.h
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef GPIO_H_
#define GPIO_H_

// c99
#include <stdint.h>

// *Note* Follow "Gpio" prefix naming convention to make it explicit 
// that these api's relate to GPIO functionality.

// *TRICKY* Opaque type hiding details of the General Purpose Input Output (GPIO) device.
// Prefer to define the struct Gpio in .c so we can change its contents in the future
struct Gpio;
// Reduce typing using typedef
typedef struct Gpio Gpio;

enum GpioType
{
	kGpioA,
	kGpioB,
	kGpioC,
	kGpioD
};
typedef enum GpioType GpioType;

// Return a pointer to the single global GPIO device instance.
Gpio* GpioGet(GpioType inGpioType);

// Constructor-like function that performs all necessary operations to initialize GPIO
void GpioInitialize(Gpio* ioGpio);

// Destructor-like function that performs all necessary operations to finalize GPIO
void GpioFinalize(Gpio* ioSim);

// Return a pointer to the GPIO device address
void* GpioGetBaseAddress(const Gpio* inGpio);

// Return the value of the PDDR register
uint32_t GpioGetRegisterPDDR(const Gpio* inGpio);

// Set the PDDR register address to hold the input value
void GpioSetRegisterPDDR(Gpio* ioGpio, uint32_t inValue);

// Return the value of the PDOR register
uint32_t GpioGetRegisterPDOR(const Gpio* inGpio);

// Set the PDOR register address to hold the input value
void GpioSetRegisterPDOR(Gpio* ioGpio, uint32_t inValue);

// Return the value of the PDIR register
uint32_t GpioGetRegisterPDIR(const Gpio* inGpio);

// Set the PDIR register address to hold the input value
void GpioSetRegisterPDIR(Gpio* ioGpio, uint32_t inValue);


#endif /* GPIO_H_ */
