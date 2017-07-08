#ifndef _DMA_H
#define _DMA_H

#include "stm32f7xx.h"
#include "dma_stream_config.h"
#include "dma_def.h"
#include "rcc.h"
#include "mpu.h"

class DMAStreamConfig;

class DMA{
	public:
		DMA(DMA_TypeDef *dma_handle);
		void init();
	
		uint8_t registerStream(DMA_Stream_TypeDef *stream, DMAStreamConfig *dmaConfig);
	
		void startStream(DMA_Stream_TypeDef *stream, uint16_t nod);
		void startStream(DMA_Stream_TypeDef *stream, uint32_t memBaseAdr, uint16_t nod);
	private:
		DMA_TypeDef *dma_handle;
};

#endif
