#ifndef MCP23S17_H_
#define MCP23S17_H_

#include <stdint.h>

#include "spi.h"

#define nEXP		4
#define nPins		16

// A or B registers
#define A			1
#define B			2
#define BOTH		3

void _mcp23s17IODIR(uint8_t pin, uint8_t dir);
void _mcp23s17IOPOL(uint8_t pin, uint8_t pol);
void _mcp23s17INTEN(uint8_t exp, uint8_t ab, uint8_t val);
void _mcp23s17DEFVAL(uint8_t exp, uint8_t ab, uint8_t val);
void _mcp23s17INTCON(uint8_t exp, uint8_t ab, uint8_t val);
void _mcp23s17IOCON(uint8_t exp, uint8_t val);
void _mcp23s17IOPU(uint8_t exp, uint8_t ab, uint8_t val);
uint8_t _mcp23s17INTF(uint8_t pin);
uint8_t _mcp23s17INTCAP(uint8_t exp, uint8_t ab);
uint8_t _mcp23s17GPIO(uint8_t exp, uint8_t ab, uint8_t reg, uint8_t read);

#endif