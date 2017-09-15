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
		void 		Init();
	
		uint8_t RegisterStream(DMA_Stream_TypeDef *stream, DMAStreamConfig *dma_config);
	
		void 		StartStream(DMA_Stream_TypeDef *stream, uint16_t nod);
		void 		StartStream(DMA_Stream_TypeDef *stream, uint32_t mem_base_adr, uint16_t nod);
	
	private:
		DMA_TypeDef *dmaHandle;
};

#endif
