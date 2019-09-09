#include <stdint.h>

#include "usb.h"
#include "adc.h"
#include "mcp3008.h"
#include "spi.h"

void adcReadAll()
{
	for(uint8_t i = 0; i != nADC; i++){
		for(uint8_t j = 0; j != nChannels; j++){
			usbdata.adc[i][j] = adcReadChannel(ADC0+i, j);
			usbAdcUpdate(i, j);
		}
	}
}

uint16_t adcReadChannel(uint8_t adc, uint8_t channel)
{
	return _mcp3008ReadChannel(adc, channel);
}