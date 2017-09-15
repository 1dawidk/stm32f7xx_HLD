#ifndef _SD_CARD_H
#define _SD_CARD_H

#include "stm32f7xx.h"
#include "sdmmc.h"
#include "spi.h"
#include "uart.h"
#include "hldkernel.h"

#define SDCARD_OK								0x0000
#define SDCARD_EC_TIMEOUT				0x1381
#define SDCARD_EC_UNKNOWN				0x1382
#define SDCARD_EC_WRONGVOLTAGE	0x1383
#define SDCARD_EC_SELECTERROR		0x1384

#define SDCARD_RCA						0x0001

#define SDCARD_CMD0						0
#define SDCARD_CMD1						1
#define SDCARD_CMD2						2
#define SDCARD_CMD3						3
#define SDCARD_CMD4						4
#define SDCARD_CMD7						7
#define SDCARD_CMD8						8
#define SDCARD_CMD8_27_36			0x00000100
#define SDCARD_CMD9						9
#define SDCARD_CMD17					17
#define SDCARD_CMD18					18
#define SDCARD_CMD24					24
#define SDCARD_CMD55					55

#define SDCARD_ACMD6					6
#define SDCARD_ACMD41					41
#define SDCARD_ACMD41_HCS			(1<<30)
#define SDCARD_ACMD41_XPC			(1<<28)
#define SDCARD_ACMD41_S18R		(1<<24)

#define SDCARD_CSD_KNOWN			1
#define SDCARD_CSD_UNKNOWN		0

class SdCard{
	public:
		enum 			State {None, Powered, Initialized, Ready, Timeout, UE, SE};

		SdCard(SDMMC *sdmmc, UART *debugUart);
		~SdCard();
		
		uint16_t 	Init(uint8_t data_bus_width);
		
		uint16_t 	GetRCA();
		uint16_t 	SetFullSpeed();
		void 			LoadCSDRegister();
	
		uint16_t 	Select();
		void 			Deselect();
		
		uint16_t 	ReadBlock(uint32_t sector, uint32_t *ext_buff);
		uint16_t 	WriteBlock(uint32_t sector, uint32_t *ext_buff);
		
		uint32_t 	GetCapacity();
		uint16_t 	GetMaxBusSpeed();
		
		uint32_t* GetCSD();

	private:
		UART 			*debugUart;
		SDMMC 		*sdmmc;
		State 		state;
	
		uint32_t 	CSD[4];
		uint16_t 	RCA;
		uint8_t 	csdKnown;
		
		
};

#endif
