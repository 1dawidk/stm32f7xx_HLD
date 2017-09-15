#ifndef _SDMMC_H
#define _SDMMC_H

#include "stm32f7xx.h"
#include "HLD/gpio.h"
#include "HLD/dma.h"
#include "hldkernel.h"

#define SDMMC_CLKCR_CLKDIV_MASK		0xFFFFFF00

#define SDMMC_400KHZ_DIV					200
#define SDMMC_50MHZ_DIV						0
#define SDMMC_25MHZ_DIV						1

#define SDMMC_BUSWIDTH_1					0x00000000
#define SDMMC_BUSWIDTH_4					0x00000800
#define SDMMC_BUSWIDTH_8					0x00010000

#define SDMMC_CPSM_FLAG_CMDREND		(1<<6)
#define SDMMC_CPSM_FLAG_CCRCFAIL	(1<<0)
#define SDMMC_CPSM_FLAG_CMDSENT		(1<<7)
#define SDMMC_CPSM_FLAG_CTIMEOUT	(1<<2)
#define SDMMC_CPSM_FLAG_CMDACT		(1<<11)
#define SDMMC_CPSM_FLAG_DONE			(0x000008C5)
#define SDMMC_CPSM_FLAG_CLEARALL	(0x000000C5)
#define SDMMC_CPSM_BUSY						0x00000002

#define SDMMC_RESPTYPE_NORESP			0x00
#define SDMMC_RESPTYPE_SHORT			0x00000040
#define SDMMC_RESPTYPE_LONG				0x000000C0

#define SDMMC_CMD_NOARGS					0x00

#define SDMMC_BLOCKSIZE_1					0x00
#define SDMMC_BLOCKSIZE_2					0x01
#define SDMMC_BLOCKSIZE_4					0x02
#define SDMMC_BLOCKSIZE_8					0x03
#define SDMMC_BLOCKSIZE_16				0x04
#define SDMMC_BLOCKSIZE_32				0x05
#define SDMMC_BLOCKSIZE_64				0x06
#define SDMMC_BLOCKSIZE_128				0x07
#define SDMMC_BLOCKSIZE_256				0x08
#define SDMMC_BLOCKSIZE_512				0x09
#define SDMMC_BLOCKSIZE_1024			0x0A
#define SDMMC_BLOCKSIZE_2048			0x0B
#define SDMMC_BLOCKSIZE_4096			0x0C
#define SDMMC_BLOCKSIZE_8192			0x0D
#define SDMMC_BLOCKSIZE_16384			0x0E

#define SDMMC_READ_FINISHED_OK		0x0000
#define SDMMC_WRITE_FINISHED_OK		0x0000
#define SDMMC_READ_INPROGRESS			0x1401
#define SDMMC_WRITE_INPROGRESS		0x1402
#define SDMMC_EC_READ_ERROR				0x1483
#define SDMMC_EC_WRITE_ERROR			0x1484

class SDMMC{
	public:
		SDMMC(SDMMC_TypeDef *sdmmc_h, DMA *dma_h);
	
		//Init
		void 			PrepareDMA();
		void 			SetBusWidth(uint32_t width);
		void 			SetCK(uint8_t state);
		void 			SetCKFreq(uint32_t sdmmc_clk_div);
		void 			SetMaxCKFreq();
		void 			SetPowerSupply(uint8_t state);
		void 			SetBlockSize(uint16_t size);
		void 			SetDataLen(uint16_t bytes);
		void 			SetTimeout(uint32_t ticks);
	
		//Data Transfer
		uint32_t 	SendCmd(uint32_t cmd, uint32_t args, uint16_t respType);
		uint8_t 	ReadCmdResponse(uint32_t *args);
		void 			PrepareToRead(uint32_t *buff);
		uint16_t 	IsReadFinished();
		void 			PrepareToWrite(uint32_t *buff);
		void			StartWrite();
		uint16_t 	IsWriteFinished();
		uint32_t 	GetReadFlags();
		
		//Other
		void 			WaitPCLK2(uint16_t n);
		
	private:
		SDMMC_TypeDef *sdmmcHandle;
		DMA 					*dmaHandle;
		uint32_t 			lastRespType;
};

#endif
