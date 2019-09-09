#include <stdint.h>

#include "spi.h"

void spiInit()
{
	DDRB = (1<<MOSI)|(1<<SCK); // Set all pins as output except MISO
	DDRA = (1<<EXP0)|(1<<EXP1)|(1<<EXP2)|(1<<EXP3)|(1<<USB0)|(1<<ADC0)|(1<<ADC1)|(1<<ADC2); // Set chip select pins as output
	PORTA = (1<<EXP0)|(1<<EXP1)|(1<<EXP2)|(1<<EXP3)|(1<<USB0)|(1<<ADC0)|(1<<ADC1)|(1<<ADC2); // Set chip select pins high
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); // SPI enable, Master mode, SPR0 for Fosc/8
	SPSR = (1<<SPI2X); // SPI double speed for Fosc/8
}

void spiSelectChip(uint8_t port)
{
	PORTA &= ~(1<<port);
}

void spiUnselect()
{
	PORTA = (1<<EXP0)|(1<<EXP1)|(1<<EXP2)|(1<<EXP3)|(1<<USB0)|(1<<ADC0)|(1<<ADC1)|(1<<ADC2); // Set all pins high
}

uint8_t spiTransfer(uint8_t data)
{
	SPDR = data; // Load the data register with the byte
	while(!(SPSR & (1<<SPIF))); // Wait until data is shifted out
	return SPDR; // Return incomming data
}

void spiWriteData(uint8_t *src, uint8_t length)
{
	while(length--)
		spiTransfer(*src++);
}

void spiReadData(uint8_t *dst, uint8_t length)
{
	while(length--)
		*dst++ = spiTransfer(0);
}