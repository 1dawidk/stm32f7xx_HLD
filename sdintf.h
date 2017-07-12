#ifndef _SD_H
#define _SD_H

/**    USAGE
 *
 * - Create SDIntf object
 * - Create SDConfig object
 * - Fill all fields SDConfig
 * - Use SDIntf "Init" method passing SDConfig
 *
 *
 * - Implement EXTI IRQ Handler:
 *        - Call SDIntf "OnCardInsert" for Sd Sense Pin <High> state
 *				- Call SDIntf "OnCardDetach" for Sd Sense Pin <Low> state
 */

#include "stm32f7xx.h"
#include "HLD/gpio.h"
#include "sd_config.h"
#include "sdCard.h"
#include "sdmmc.h"
#include "exti.h"
#include "hldkernel.h"
#include "uart.h"
#include "dma.h"
#include "dma_stream_config.h"
#include "rcc.h"
#include "FATPartition.h"

#define SD_CARD_INSERTED						1
#define SD_NO_CARD									0

#define SDINTF_BUSWIDTH_1						0x00
#define SDINTF_BUSWIDTH_4						0x01

#define SDINTF_INIT_TRIES						3

//Error Codes
#define SDINTF_OK										0x0000
#define SDINTF_EC_UNKNOWN_ERROR			0x0F81
#define SDINTF_EC_MOUNT_ERROR				0x0F82
#define SDINTF_EC_CARD_ERROR				0x0F83

//SD CARD State
#define SDINTF_STATE_IDLE						1
#define SDINTF_STATE_CARD_DETECTED	2
#define SDINTF_STATE_CARD_READY			3
#define SDINTF_STATE_CARD_ERROR			4
#define SDINTF_STATE_OFF						5

//MBR structure definition (PART - partition)
#define MBR_PART0_START							0x1be
#define MBR_PART1_START							0x1ce
#define MBR_PART2_START							0x1de
#define MBR_PART3_START							0x1ee
#define MBR_PART_STATUS_OFFSET			0x00
#define MBR_PART_CHSSTART_OFFSET		0x01
#define MBR_PART_TYPE_OFFSET				0x04
#define MBR_PART_CHSEND_OFFSET			0x05
#define MBR_PART_LBASTART_OFFSET		0x08
#define MBR_PART_SIZE_OFFSET				0x0c

class SDIntf{
	public:	
		//Methods
		SDIntf(SDMMC_TypeDef *sdmmc_h, UART *uart, DMA *dma2);
		void Init(SDConfig *sdConfig);
		void CardDetectHandler();
		uint16_t IsCardReady();
		
		uint16_t MountFAT(uint8_t pId);
	
		FATPartition* GetFATPartition();
	
		//Handlers
		void OnCardInsert();
		void OnCardDetach();

		//Dump
		uint32_t* GetCSD();
	private:
		SDMMC *sdmmc;
		SdCard *sdCard;
		FATPartition *partition;
		uint8_t stateMachine;
	
		UART *uart;
	
		GPIO_TypeDef *senseSeg;
		uint16_t sensePin;
		uint8_t busWidth;
		
};

#endif
