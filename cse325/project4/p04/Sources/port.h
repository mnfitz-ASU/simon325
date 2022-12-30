//************************************************************************************************************
// File: port.h
// Project 4
// Name: Matthew Fitzgerald # 1216579507 (mnfitzg1@asu.edu)
// Class: CSE325 Embedded Microprocessor Systems
// Semester: Fall 2021
// Arizona State University, Tempe AZ 85287-8809
//************************************************************************************************************
#ifndef PORT_H_
#define PORT_H_

// c99
#include <stdint.h>

// *Note* Follow "Port" prefix naming convention to make it explicit 
// that these api's relate to GPIO functionality.

// *TRICKY* Opaque type hiding details of the Peripheral PORTD (PORT) device.
// Prefer to define the struct Port in .c so we can change its contents in the future
struct Port;
// Reduce typing using typedef
typedef struct Port Port;

enum PortType
{
	kPortA,
	kPortB,
	kPortD
};
// Reduce typing using typedef
typedef enum PortType PortType;

// Return a pointer to the specified PORT device instance.
Port* PortGet(PortType inPortType);

// Constructor-like function that performs all necessary operations to initialize Port
void PortInitialize(Port* ioPort);

// Destructor-like function that performs all necessary operations to finalize Port
void PortFinalize(Port* ioPort);

// Return a pointer to the GPIO device address
void* PortGetBaseAddress(const Port* inPort);

// Return the location of the PCR register's given pin
uint32_t PortGetRegisterPCR(const Port* inPort, int inPinNumber);

// Set the pin at the PCR register address to hold the input value
void PortSetRegisterPCR(Port* ioPort, int inPinNumber, uint32_t inValue);

#endif /* PORT_H_ */
