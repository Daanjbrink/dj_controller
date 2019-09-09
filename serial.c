#include <avr/io.h>
#include <util/delay.h>

#include "serial.h"

void initSerial()
{
	UBRR0H = (BAUDRATE>>8);
	UBRR0L = BAUDRATE;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Enable transmitter
}

void SerialWrite(char *c, short len)
{
	while(len--)
		SerialWriteByte(*c++);
}

void SerialWriteByte(char c)
{
	while(!(UCSR0A & (1<<UDRE0))); // Wait until UART is ready
	UDR0 = c;
	while(!(UCSR0A & (1<<UDRE0))); // Wait until byte is shifted out
}