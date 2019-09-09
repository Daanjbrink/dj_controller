#ifndef MAX3421E_H_
#define MAX3421E_H_

#include <stdint.h>

#include "usb.h"

void _max3421eInit();

uint16_t _max3421eRegister(uint8_t reg, uint8_t val, uint8_t read);

#endif