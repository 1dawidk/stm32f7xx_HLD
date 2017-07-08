#ifndef _USB_CDC_H
#define _USB_CDC_H

#include "stm32f745xx.h"
#include "HighLayer/usb.h"

//#define CDC_IN_EP                                   0x81  /* EP1 for data IN */
//#define CDC_OUT_EP                                  0x01  /* EP1 for data OUT */
//#define CDC_CMD_EP                                  0x82  /* EP2 for CDC commands */

/* CDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
//#define CDC_DATA_HS_MAX_PACKET_SIZE                 512  /* Endpoint IN & OUT Packet size */
//#define CDC_DATA_FS_MAX_PACKET_SIZE                 64  /* Endpoint IN & OUT Packet size */
//#define CDC_CMD_PACKET_SIZE                         8  /* Control Endpoint Packet size */ 

//#define USB_CDC_CONFIG_DESC_SIZ                     67
//#define CDC_DATA_HS_IN_PACKET_SIZE                  CDC_DATA_HS_MAX_PACKET_SIZE
//#define CDC_DATA_HS_OUT_PACKET_SIZE                 CDC_DATA_HS_MAX_PACKET_SIZE

//#define CDC_DATA_FS_IN_PACKET_SIZE                  CDC_DATA_FS_MAX_PACKET_SIZE
//#define CDC_DATA_FS_OUT_PACKET_SIZE                 CDC_DATA_FS_MAX_PACKET_SIZE

/*---------------------------------------------------------------------*/
/*  CDC definitions                                                    */
/*---------------------------------------------------------------------*/
//#define CDC_SEND_ENCAPSULATED_COMMAND               0x00
//#define CDC_GET_ENCAPSULATED_RESPONSE               0x01
//#define CDC_SET_COMM_FEATURE                        0x02
//#define CDC_GET_COMM_FEATURE                        0x03
//#define CDC_CLEAR_COMM_FEATURE                      0x04
//#define CDC_SET_LINE_CODING                         0x20
//#define CDC_GET_LINE_CODING                         0x21
//#define CDC_SET_CONTROL_LINE_STATE                  0x22
//#define CDC_SEND_BREAK                              0x23

class USB_cdc{
	public:
		//Variables
		USB_cdc(USB* usb);
	
		//Methods
		uint8_t registerInterface();
		uint8_t setTxBuffer(uint8_t  *txBuff, uint16_t l);
		uint8_t setRxBuffer(uint8_t  *rxBuff);
		uint8_t receivePacket();
		uint8_t transmitPacket();
	private:
		//Variables
		USB* usb;
	
		//Methods
		uint8_t  init(uint8_t cfgidx);
		uint8_t  deInit(uint8_t cfgidx);
		//uint8_t  setup(USBD_SetupReqTypedef *req);
		uint8_t  dataIn(uint8_t epnum);
		uint8_t  dataOut(uint8_t epnum);
		uint8_t  EP0_RxReady();
		uint8_t* getFSCfgDesc(uint16_t *length);
		uint8_t* getHSCfgDesc(uint16_t *length);
		uint8_t* getOtherSpeedCfgDesc(uint16_t *length);
		uint8_t* getDeviceQualifierDescriptor(uint16_t *length);
};

#endif
