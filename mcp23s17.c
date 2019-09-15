#include <util/atomic.h>
#include <stdint.h>

#include "mcp23s17.h"
#include "spi.h"

static uint8_t _mcp23s17Register(uint8_t exp, uint8_t reg, uint8_t val, uint8_t read)
{
	 uint8_t buffer[3] = {(1<<6)|!!(read), // Set bit 6, set bit 0 accordingly
						 reg,  // Register address
						 val}; // Register value

	uint8_t ret;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		spiSelectChip(exp);
		spiTransfer(buffer[0]); // Read/write operation
		spiTransfer(buffer[1]); // Register address
		ret = spiTransfer(buffer[2]); // Read/write data
		spiUnselect();
	}
	return ret;	
}

void _mcp23s17IODIR(uint8_t pin, uint8_t dir);
void _mcp23s17IOPOL(uint8_t pin, uint8_t pol);

void _mcp23s17INTEN(uint8_t exp, uint8_t ab, uint8_t val)
{
	if(ab&1)
		_mcp23s17Register(exp, 0x04, val, 0);
	if(ab&2)
		_mcp23s17Register(exp, 0x05, val, 0);
}

void _mcp23s17DEFVAL(uint8_t exp, uint8_t ab, uint8_t val)
{
	if(ab&1)
		_mcp23s17Register(exp, 0x06, val, 0);
	if(ab&2)
		_mcp23s17Register(exp, 0x07, val, 0);
}

void _mcp23s17INTCON(uint8_t exp, uint8_t ab, uint8_t val)
{
	if(ab&1)
		_mcp23s17Register(exp, 0x08, val, 0);
	if(ab&2)
		_mcp23s17Register(exp, 0x09, val, 0);
}

void _mcp23s17IOCON(uint8_t exp, uint8_t val)
{
	// The A and B registers are the same
	_mcp23s17Register(exp, 0x0A, val, 1);
}

void _mcp23s17IOPU(uint8_t exp, uint8_t ab, uint8_t val)
{
	if(ab&1)
		_mcp23s17Register(exp, 0x0C, val, 0);
	if(ab&2)
		_mcp23s17Register(exp, 0x0D, val, 0);
}

uint8_t _mcp23s17INTF(uint8_t pin);

uint8_t _mcp23s17INTCAP(uint8_t exp, uint8_t ab)
{
	if(ab&1)
		return _mcp23s17Register(exp, 0x10, 0, 1);
	else if(ab&2)
		return _mcp23s17Register(exp, 0x11, 0, 1);
}

uint8_t _mcp23s17GPIO(uint8_t exp, uint8_t ab, uint8_t pin, uint8_t read)
{
	uint8_t reg = 0xFF;
	if(ab&1)
		reg = 0x12;
	else if(ab&2)
		reg = 0x13;
	return _mcp23s17Register(exp, reg, pin, read);
}