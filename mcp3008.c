#include <util/atomic.h>
#include <stdint.h>

#include "mcp3008.h"
#include "spi.h"

uint16_t _mcp3008ReadChannel(uint8_t adc, uint8_t channel)
{
	// Buffer according to MCP3008 datasheet figure 6-1
	uint8_t buffer[2] =	{1, // Set start bit as lowest bit
						(1<<SGLDIFF)|((channel&0b111)<<4)}; // Set single/diff bit, put 3 bits of channel in bits 4,5,6, use &0b111 to only select 3 lower bits
	
	uint8_t upperbits;
	uint8_t lowerbits;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		spiSelectChip(adc);		
		spiTransfer(buffer[0]); // Transfer start bit
		upperbits = spiTransfer(buffer[1]); // Transfer channel bits and retrieve upper 2 bits of result
		lowerbits = spiTransfer(0); // Dummy load
		spiUnselect();
	}
	return (((upperbits&0b11)<<8)|lowerbits); // Return 10 bit value
}