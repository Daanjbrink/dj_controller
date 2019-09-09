#include <util/atomic.h>
#include <stdint.h>

#include "usb.h"
#include "max3421e.h"
#include "spi.h"

void _max3421eInit();

uint16_t _max3421eRegister(uint8_t reg, uint8_t val, uint8_t read)
{
	uint8_t buffer[2] = {((reg&0b1111)<<3)|((!read) ? 2 : 0)|1, // Set register address in bits 7..3, set bit 2 to 1 for writing or 0 for reading, set bit 1 for ackstat
						val}; // Data byte
	uint8_t status;
	uint8_t regval;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		spiSelectChip(USB0);
		status = spiTransfer(buffer[0]); // Write command byte, retrieve status byte
		regval = spiTransfer(buffer[1]); // Write register data, retrieve register data
		spiUnselect();
	}	
	return ((status<<8)|regval); // Return the usb status in the upper 8 bits, register value in lower 8 bits
}