#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>

#define LEFT	1
#define MID		2
#define RIGHT	3

#define MOSI	PORTB5
#define MISO	PORTB6
#define SCK		PORTB7

#define EXP0	PORTA0
#define EXP1	PORTA1
#define EXP2	PORTA2
#define EXP3	PORTA3
#define USB0	PORTA4
#define ADC0	PORTA5
#define ADC1	PORTA6
#define ADC2	PORTA7

void spiInit();

void spiSelectChip(uint8_t port);
void spiUnselect();

uint8_t spiTransfer(uint8_t data);

void spiReadData(uint8_t *data, uint8_t length);
void spiWriteData(uint8_t *data, uint8_t length);

#endif