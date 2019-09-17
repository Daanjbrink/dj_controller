#ifndef MAX3421E_H_
#define MAX3421E_H_

#include <stdint.h>

#include "usb.h"

#define EP0FIFO 0
#define EP1FIFO 1
#define EP2FIFO 2
#define EP3FIFO 3
#define SUDFIFO 4

#define CONNECT 3

void max3421eInit();

void max3421eWriteBuffer(uint8_t infifo, uint8_t *buffer, uint8_t len);
void max3421eReadBuffer(uint8_t outfifo, uint8_t* buffer, uint8_t len);

void max3421eACKSTAT();
void max3421eEPSTALLS(uint8_t bitmask);
uint8_t max3421eEPIRQ(uint8_t fifo, uint8_t inout);
void max3421eEPIEN(uint8_t bitmask);
void max3421eUSBCTL(uint8_t bitmask);

#endif