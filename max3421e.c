#include <util/atomic.h>
#include <stdint.h>

#include "usb.h"
#include "max3421e.h"
#include "spi.h"

static uint16_t _max3421eRegister(uint8_t reg, uint8_t val, uint8_t read)
{
	uint8_t buffer[2] = {((reg&0b1111)<<3)|((!read) ? 2 : 0)|1, // Set register address in bits 7..3, set bit 2 for writing or unset for reading, set bit 1 for ackstat
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
		while(len--){
			if(read){
				*data = spiTransfer(0); // Write register content to buffer
			} else {
				spiTransfer(*data); // Write buffer content to register
			}
			data++; // Increment pointer address
		}
		spiUnselect();
	}
	return status; // Return the usb status in the upper 8 bits, register value in lower 8 bits
}

void _max3421eInit()
{
	_max3421eRegister(17, (1<<4), 0); // Set SPI in full-duplex mode
}

// infifo is the FIFO register used to transfer outgoing data
// infifo can be 0, 2, 3
void _max3421eWriteBulk(uint8_t infifo, uint8_t *buffer, uint8_t len)
{
	_max3421eRegisterBuffer(infifo, buffer, len, 0); // Write IN FIFO buffer
	_max3421eRegister(infifo+5, len, 0); // Write IN FIFO bytecount register which arms the endpoint for transfer
										 // The bytecount registers are 5 addresses higher than the FIFO register addresses
}