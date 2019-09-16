#include "max3421e.h"
#include "max3421eSetup.h"

void _max3421eSetupUsb()
{
	char setupData[8];

	_max3421eReadBulk(4, setupData, 8); // Read SUDFIFO buffer
	
	// switch setupdata
	switch(setupData[0]){
		case 0:
			
			break;
	}
	
	_max3421eUSBCTL((1<<CONNECT)); // Set connect bit
}