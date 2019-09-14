#ifndef MAX3421E_H_
#define MAX3421E_H_

#include <stdint.h>

#include "usb.h"

void _max3421eInit();

void _max3421eWriteBulk(uint8_t infifo, uint8_t *buffer, uint8_t len);

#endif