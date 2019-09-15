#include <avr/interrupt.h>

#include <stdint.h>
#include <string.h>

#include "usb.h"
#include "exp.h"
#include "mcp23s17.h"

void expInit()
{
	PCICR = (1<<PCIE2); // Enable pin change interrupt on pins PC0..7
	PCMSK2 = 0xFF; // Enable pin change mask on pins PC0..7

	for(uint8_t i = 0; i != nEXP; i++)
		_mcp23s17IOCON(EXP0+i, (1<<5)); // set SEQOP bit

	// Set registers on every mcp23s17 except EXP3
	for(uint8_t i = 0; i != nEXP-1; i++){
		_mcp23s17IOPU(EXP0+i, BOTH, 0xFF); // Enable internal pull ups on every pin
		_mcp23s17INTEN(EXP0+i, BOTH, 0xFF); // Enable interrupt-on-change on every pin
	}

	// Rotary encoders EXP3
	_mcp23s17IOPU(EXP3, B, 0xFF); // Enable internal pull ups on deck B on every pin
	_mcp23s17INTEN(EXP3, BOTH, 0xFF); // Enable interrupt-on-change on every pin

	// Init current encoder state
	encoderState[0] = expReadPin(EXP3, 2);
	encoderState[1] = expReadPin(EXP3, 4);
	encoderState[2] = expReadPin(EXP3, 6);
}

// Read gpio pin from io expander ic
// pins 0..7 are deck A
// pins 8..15 are deck B
uint8_t expReadPin(uint8_t exp, uint8_t pin)
{
	uint8_t ab;
	if(pin >= 7){
		ab = B;
		pin -= 8; // 8-15
	} else {
		ab = A;
	}
	return (_mcp23s17GPIO(exp, ab, 0, 1)&(1<<pin));
}

// ISRs should be short but since this one isn't, optimize always
__attribute__((optimize("O3"))) ISR(PCINT2_vect)
{
	// Check which IO expander pin caused the interrupt
	uint8_t exp = 0, ab = 0;
	if(PINC&1){
		exp = EXP0;
		ab = A;
	} else if(PINC&2){
		exp = EXP0;
		ab = B;
	} else if(PINC&4){
		exp = EXP1;
		ab = A;
	} else if(PINC&8){
		exp = EXP1;
		ab = B;
	} else if(PINC&16){
		exp = EXP2;
		ab = A;
	} else if(PINC&32){
		exp = EXP2;
		ab = B;
	} else if(PINC&64){
		exp = EXP3;
		ab = A;
	} else if(PINC&128){
		exp = EXP3;
		ab = B;
	}

	uint8_t reg = _mcp23s17INTCAP(exp, ab); // Get register that caused interrupt
	uint8_t pin = 0; // The pin that caused the interrupt
	if(reg&1)
		pin = 0;
	else if(reg&2)
		pin = 1;
	else if(reg&4)
		pin = 2;
	else if(reg&8)
		pin = 3;
	else if(reg&16)
		pin = 4;
	else if(reg&32)
		pin = 5;
	else if(reg&64)
		pin = 6;
	else if(reg&128)
		pin = 7;

	// Rotary encoders
	if(exp == EXP3 && ab == A){
		switch(pin){
			case 2:
				encoderState[0] = !encoderState[0];
				if(expReadPin(exp, 3) != encoderState[0])
					usbBuffer[7]++;
				else
					usbBuffer[7]--;
				break;

			case 4:
				encoderState[1] = !encoderState[1];
				if(expReadPin(exp, 5) != encoderState[1])
					usbBuffer[8]++;
				else
					usbBuffer[8]--;
				break;

			case 6:
				encoderState[2] = !encoderState[2];
				if(expReadPin(exp, 7) != encoderState[2])
					usbBuffer[9]++;
				else
					usbBuffer[9]--;
				break;
		}
		return;
	}

	if(ab == B)
		pin += 8; // Pins 8-15 are on deck B

	// Toggle pressed pin in data struct and update usb buffer
	usbdata.btn[exp][pin] = !usbdata.btn[exp][pin];
	usbBtnUpdate(exp, pin);
}