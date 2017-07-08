#ifndef _DMA_STREAM_CONF_H
#define _DMA_STREAM_CONF_H

#include "dma_def.h"
#include "stm32f745xx.h"

class DMAStreamConfig{
	public:
		uint32_t direction;
		uint32_t periphAdr;
		uint32_t memoryAdr;
		uint32_t ch;
		uint32_t priority;
		uint8_t fc;
		uint8_t memDataSize;
		uint8_t periphDataSize;
		uint8_t periphInc;
		uint8_t memInc;
		uint8_t circural;
		uint8_t mburst;
		uint8_t pburst;
	
		DMAStreamConfig();
};

#endif
