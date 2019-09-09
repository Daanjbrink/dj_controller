#include <stdint.h>

#include "usb.h"
#include "max3421e.h"
#include "spi.h"

// Transfer byte array of 8 bytes containing only '1' or '0' to 1 byte
static uint8_t byteArrayToByte(uint8_t *array)
{
	uint8_t tmp = 0;
	for(uint8_t i = 0; i != 8; i++)
		tmp |= (*(array+i))<<i;
	return tmp;
}

void usbBtnUpdate(uint8_t exp, uint8_t pin)
{
	// Map expansion IC number and pin number to usb buffer index
	uint8_t a = 0, b = 0;	
	if(pin >= 8 && pin <= 15){
		a = 1;
		b = 8;
	}
	usbBuffer[(exp*2)+a] = byteArrayToByte(&usbdata.btn[exp][b]);
}

void usbAdcUpdate(uint8_t adc, uint8_t channel)
{
	// usbBuffer adc base index
	uint8_t i = 0;
	if(adc == 0)
		i = 10;
	else if(adc == 1)
		i = 24;
	else if(adc == 2)
		i = 36;

	i += 2*channel; // Get usbBuffer adc index

	usbBuffer[i] = usbdata.adc[adc][channel];
	usbBuffer[i+1] = usbdata.adc[adc][channel]>>8;
}

void usbWriteBuffer()
{
	
}

/*void usbWriteStruct()
{
	// https://www.virtualdj.com/wiki/HIDImplementation.html
	
	// Left deck push buttons - 16 buttons
	usbBuffer[0] = byteArrayToByte(&usbdata.btn[0][0]);
	usbBuffer[1] = byteArrayToByte(&usbdata.btn[0][8]);
	
	// Right deck push buttons - 16 buttons
	usbBuffer[2] = byteArrayToByte(&usbdata.btn[1][0]);
	usbBuffer[3] = byteArrayToByte(&usbdata.btn[1][8]);
	
	// Middle deck push buttons - 14 buttons
	usbBuffer[4] = byteArrayToByte(&usbdata.btn[2][0]);
	usbBuffer[5] = byteArrayToByte(&usbdata.btn[2][8]); 
	
	// Rotary encoder clicks - 4 buttons
	usbBuffer[6] = byteArrayToByte(&usbdata.btn[3][8]);
	
	// Rotary encoders - 3 encoders
	usbBuffer[7] = usbdata.enc[0];
	usbBuffer[8] = usbdata.enc[1];
	usbBuffer[9] = usbdata.enc[2];
	
	// Left deck pots - 7 pots
	usbBuffer[10] = usbdata.adc[0][0];
	usbBuffer[11] = usbdata.adc[0][0]>>8;
	usbBuffer[12] = usbdata.adc[0][1];
	usbBuffer[13] = usbdata.adc[0][1]>>8;
	usbBuffer[14] = usbdata.adc[0][2];
	usbBuffer[15] = usbdata.adc[0][2]>>8;
	usbBuffer[16] = usbdata.adc[0][3];
	usbBuffer[17] = usbdata.adc[0][3]>>8;
	usbBuffer[18] = usbdata.adc[0][4];
	usbBuffer[19] = usbdata.adc[0][4]>>8;
	usbBuffer[20] = usbdata.adc[0][5];
	usbBuffer[21] = usbdata.adc[0][5]>>8;
	usbBuffer[22] = usbdata.adc[0][6]; // X-fader, only on left deck
	usbBuffer[23] = usbdata.adc[0][6]>>8;
	
	// Right deck pots - 6 pots
	usbBuffer[24] = usbdata.adc[1][0];
	usbBuffer[25] = usbdata.adc[1][0]>>8;
	usbBuffer[26] = usbdata.adc[1][1];
	usbBuffer[27] = usbdata.adc[1][1]>>8;
	usbBuffer[28] = usbdata.adc[1][2];
	usbBuffer[29] = usbdata.adc[1][2]>>8;
	usbBuffer[30] = usbdata.adc[1][3];
	usbBuffer[31] = usbdata.adc[1][3]>>8;
	usbBuffer[32] = usbdata.adc[1][4];
	usbBuffer[33] = usbdata.adc[1][4]>>8;
	usbBuffer[34] = usbdata.adc[1][5];
	usbBuffer[35] = usbdata.adc[1][5]>>8;
	
	// Mid deck pots - 8 pots
	usbBuffer[36] = usbdata.adc[2][0];
	usbBuffer[37] = usbdata.adc[2][0]>>8;
	usbBuffer[38] = usbdata.adc[2][1];
	usbBuffer[39] = usbdata.adc[2][1]>>8;
	usbBuffer[40] = usbdata.adc[2][2];
	usbBuffer[41] = usbdata.adc[2][2]>>8;
	usbBuffer[42] = usbdata.adc[2][3];
	usbBuffer[43] = usbdata.adc[2][3]>>8;
	usbBuffer[44] = usbdata.adc[2][4];
	usbBuffer[45] = usbdata.adc[2][4]>>8;
	usbBuffer[46] = usbdata.adc[2][5];
	usbBuffer[47] = usbdata.adc[2][5]>>8;
	usbBuffer[48] = usbdata.adc[2][6];
	usbBuffer[49] = usbdata.adc[2][6]>>8;
	usbBuffer[50] = usbdata.adc[2][7];
	usbBuffer[51] = usbdata.adc[2][7]>>8;

	spiWriteData(usbBuffer, sizeof(usbBuffer)); // Make sure the above code gets compiled by doing something with it
}*/