#ifndef USB_H_
#define USB_H_

#include <stdint.h>

// https://www.virtualdj.com/wiki/HIDImplementation.html

struct usbData {
	uint8_t btn[4][16]; // 0 = left side, 1 = right side, 2 = mid side, 3 = rotary encoders
	uint16_t adc[3][8]; // 0 = left side, 1 = right side, 2 = mid side
	uint8_t enc[3];		// 0 = left side, 1 = right side, 2 = mid side
} usbdata;

uint8_t usbBuffer[51];

void usbInit();
void usbBtnUpdate(uint8_t exp, uint8_t pin);
void usbAdcUpdate(uint8_t adc, uint8_t channel);
void usbWriteBuffer();

#endif