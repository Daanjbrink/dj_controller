#include "max3421e.h"
#include "max3421eSetup.h"

struct usbRequest{
	uint8_t type;
	uint8_t request;
	uint8_t valuel;
	uint8_t valueh;
	uint16_t index;
	uint16_t length;
};

static void setupAckstat()
{
	max3421eACKSTAT();
}

static void setupStall()
{
	max3421eEPSTALLS((1<<5));
}

static void setupRequestRecpDevice(struct usbRequest *setupData)
{
	switch(setupData->request){
		case REQUEST_GET_STATUS:;
			uint8_t buffer[2] = {0, 0};
			max3421eWriteBuffer(EP0FIFO, buffer, 2); // Write 2 zero bytes since this device isn't self-powered nor has it been enabled for remote wakeup
			break;

		case REQUEST_CLEAR_FEATURE:
			// Doesn't apply to this device
			setupAckstat();
			break;

		case REQUEST_SET_FEATURE:
			// Doesn't apply to this device
			setupAckstat();
			break;

		case REQUEST_SET_ADDRESS:
			// The max3421e handles this by hardware
			setupAckstat();
			break;

		case REQUEST_GET_DESCRIPTOR:
			
			break;

		case REQUEST_SET_DESCRIPTOR:
			// Invalid command
			setupStall();
			break;

		case REQUEST_GET_CONFIGURATION:
		
			break;

		case REQUEST_SET_CONFIGURATION:
		
			break;
	
		case REQUEST_GET_INTERFACE:
		
			break;

		case REQUEST_SET_INTERFACE:
		
			break;

		case REQUEST_SYNCH_FRAME:
		
			break;
			
		default:
			setupStall();
			break;
	}
}

static void setupRequestTypeStandard(struct usbRequest *setupData)
{
	// Switch request recipient which is stored in the lower 4 bits
	switch(setupData->type & 0xF){
		case REQUEST_RECP_DEVICE:
			setupRequestRecpDevice(setupData);
			break;
		
		default:
			setupStall();
			break;
	}
}

static void setupRequestTypeClass(struct usbRequest *setupData)
{
	
}

void max3421eSetupUsb()
{
	struct usbRequest setupData;

	max3421eReadBuffer(4, (uint8_t *)&setupData, 8); // Read SUDFIFO buffer
	
	// Switch request type stored in bits 5 and 6
	switch(setupData.type & 0x60){
		case REQUEST_TYPE_STANDARD:
			setupRequestTypeStandard(&setupData);
			break;
		
		case REQUEST_TYPE_CLASS:
			setupRequestTypeClass(&setupData);
			break;
		
		default:
			setupStall();
			break;
	}
	
	max3421eUSBCTL((1<<CONNECT)); // Set connect bit to start the usb traffic
}