#ifndef MCP3008_H_
#define MCP3008_H_

#include <stdint.h>

#define SGLDIFF 7

uint16_t _mcp3008ReadChannel(uint8_t adc, uint8_t channel);

#endif