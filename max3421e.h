#ifndef MAX3421E_H_
#define MAX3421E_H_

#include <stdint.h>

#include "usb.h"

void _max3421eInit();

void _max3421eWriteBulk(uint8_t infifo, uint8_t *buffer, uint8_t len);
void _max3421eReadBulk(uint8_t outfifo, uint8_t* buffer, uint8_t len);

uint8_t _max3421eEPIRQ(uint8_t fifo, uint8_t inout);
void _max3421eEPIEN(uint8_t bitmask);

#endif