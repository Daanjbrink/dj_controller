#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#include "usb.h"

#define nADC		3
#define nChannels	8

void adcReadAll();
uint16_t adcReadChannel(uint8_t adc, uint8_t channel);

#endif