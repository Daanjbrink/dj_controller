#include <util/atomic.h>
#include <stdint.h>

#include "usb.h"
#include "max3421e.h"
#include "max3421eSetup.h"
#include "spi.h"

static uint16_t _max3421eRegister(uint8_t reg, uint8_t val, uint8_t read)
{
	uint8_t buffer[2] = {((reg&0b1111)<<3)|((read) ? 0 : 2)|1, // Set register address in bits 7..3, set bit 2 for writing or unset for reading, set bit 1 for ackstat
						val}; // Data byte

	uint8_t status, regval;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		spiSelectChip(USB0);
		status = spiTransfer(buffer[0]); // Write command byte, retrieve status byte
		regval = spiTransfer(buffer[1]); // Write register data, retrieve register data
		spiUnselect();
	}
	return ((status<<8)|regval); // Return the usb status in the upper 8 bits, register value in lower 8 bits
}

static uint8_t _max3421eRegisterBuffer(uint8_t reg, uint8_t *data, uint8_t len, uint8_t read)
{
	uint8_t buffer = {((reg&0b1111)<<3)|1}; // Set register address in bits 7..3, set bit 2 for writing or unset for reading, set bit 1 for ackstat

	uint8_t status;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		spiSelectChip(USB0);
		status = spiTransfer(buffer); // Write command byte, retrieve status byte
		if(read){
			spiReadData(data, len);
		} else {
			spiWriteData(data, len);
		}
		spiUnselect();
	}
	return status; // Return the usb status in the upper 8 bits, register value in lower 8 bits
}

void _max3421eInit()
{
	_max3421eRegister(17, (1<<4), 0); // Set SPI in full-duplex mode
	_max3421eSetupUsb();
}

// infifo is the FIFO register used to transfer outgoing data
// infifo can be 0, 2, 3
void _max3421eWriteBulk(uint8_t infifo, uint8_t *buffer, uint8_t len)
{
	_max3421eRegisterBuffer(infifo, buffer, len, 0); // Write IN FIFO buffer
	_max3421eRegister(infifo+5, len, 0); // Write IN FIFO bytecount register which arms the endpoint for transfer
										 // The bytecount registers are 5 addresses higher than the FIFO register addresses
}

// outfifo is the FIFO register used to transfer outgoing data
// outfifo can be 0, 1
void _max3421eReadBulk(uint8_t outfifo, uint8_t* buffer, uint8_t len)
{
	uint8_t bytecount = _max3421eRegister(outfifo+5, 0, 1); // Read byte count
	bytecount = (bytecount > len ? len : bytecount); // bytecount is less or equal to len
	_max3421eRegisterBuffer(outfifo, buffer, bytecount, 1); // Read OUT FIFO buffer
	_max3421eRegister(11, (1<<(outfifo+1)), 0); // Write '1' to DAV IRQ register. Bits used are b1 or b2
}

// Checks if specified IRQ flag is set
// FIFO 0 has 2 flags
uint8_t _max3421eEPIRQ(uint8_t fifo, uint8_t inout)
{
	uint8_t bit;
	// FIFO 0 is an in/out buffer
	if(fifo == 0){
		// in is 0
		// out is 1
		if(inout == 0){
			bit = 0;
		} else {
			bit = 1;
		}
	} else {
		bit = fifo+1; // Because FIFO 0 is a bi-directional buffer and requires 2 bits, all the other bits are 1 bit position higher than their fifo number
	}
	return !!(_max3421eRegister(11, (1<<bit), 1)); // Returns '1' if specified IRQ flag is set, otherwise '0'
}

// Enable FIFO interrupts
void _max3421eEPIEN(uint8_t bitmask)
{
	_max3421eRegister(12, bitmask, 0);
}

// Writes to the USBCTL register
void _max3421eUSBCTL(uint8_t bitmask)
{
	_max3421eRegister(15, bitmask, 0);
}