#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>

#include "spi.h"
#include "serial.h"
#include "adc.h"
#include "exp.h"

int main()
{
	_delay_ms(2000); // Wait for python terminal to startup
	
	#ifdef DEBUG
	initSerial();
	#endif
	
	// Init all hardware
	spiInit();
	usbInit();
	expInit();
	
	sei(); // Enable global interrupts
	
    while (1)
	{
		// Continuously update adc values and output the usb buffer
		// Button presses and encoders are done via interrupts
		adcReadAll();
		usbWriteBuffer();
    }
}

