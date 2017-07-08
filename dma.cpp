#include "dma.h"

DMA::DMA(DMA_TypeDef *dma_handle){
	this->dma_handle= dma_handle;
}

void DMA::init(){
	if(dma_handle==DMA1){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_DMA1, ENABLE);
	} else if(dma_handle==DMA2){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_DMA2, ENABLE);
	}
}

uint8_t DMA::registerStream(DMA_Stream_TypeDef *stream, DMAStreamConfig *dmaConfig){
	stream->CR&= ~DMA_SxCR_EN;
	
	//Set peripheral address
	stream->PAR= dmaConfig->periphAdr;
	//Set memory address
	stream->M0AR=dmaConfig->memoryAdr;
	
	
															
	//Set channel, priority, direction, burst
	stream->CR|= dmaConfig->ch | dmaConfig->priority | dmaConfig->direction;
	stream->CR|=  ((uint32_t)dmaConfig->memDataSize<<13) | ((uint32_t)dmaConfig->periphDataSize<<11);
	stream->CR|= ((uint32_t)dmaConfig->periphInc<<9) | ((uint32_t)dmaConfig->memInc<<10) | ((uint32_t)dmaConfig->circural<<8);
	stream->CR|= ((uint32_t)dmaConfig->mburst<<23) | ((uint32_t)dmaConfig->pburst<<21);
															
	//Set flow controller
	if(dmaConfig->fc)
		stream->CR|=DMA_SxCR_PFCTRL;
	
	//Set FIFO
	if(dmaConfig->mburst!=DMA_BURST_SINGLE || dmaConfig->pburst!=DMA_BURST_SINGLE){
		stream->FCR= DMA_SxFCR_DMDIS | DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1;
	}
	
	return 0;
}


//Clear flags, Set new Number of Data, Enable Stream
void DMA::startStream(DMA_Stream_TypeDef *stream, uint16_t nod){
	if(!(stream->CR&DMA_SxCR_EN)){
		if(stream==DMA1_Stream0 || stream==DMA2_Stream0){
			dma_handle->LIFCR|= (0x0000003D<<0);
		} else if(stream==DMA1_Stream1 || stream==DMA2_Stream1){
			dma_handle->LIFCR|= (0x0000003D<<6);
		}else if(stream==DMA1_Stream2 || stream==DMA2_Stream2){
			dma_handle->LIFCR|= (0x0000003D<<16);
		}else if(stream==DMA1_Stream3 || stream==DMA2_Stream3){
			dma_handle->LIFCR|= (0x0000003D<<22);
		}else if(stream==DMA1_Stream4 || stream==DMA2_Stream4){
			dma_handle->HIFCR|= (0x0000003D<<0);
		}else if(stream==DMA1_Stream5 || stream==DMA2_Stream5){
			dma_handle->HIFCR|= (0x0000003D<<6);
		}else if(stream==DMA1_Stream6 || stream==DMA2_Stream6){
			dma_handle->HIFCR|= (0x0000003D<<16);
		}else if(stream==DMA1_Stream7 || stream==DMA2_Stream7){
			dma_handle->HIFCR|= (0x0000003D<<22);
		}
		
		stream->NDTR= nod;
		
		stream->CR|=DMA_SxCR_EN;
	}
}

void DMA::startStream(DMA_Stream_TypeDef *stream, uint32_t memBaseAdr, uint16_t nod){
	if(!(stream->CR&DMA_SxCR_EN)){
		if(stream==DMA1_Stream0 || stream==DMA2_Stream0){
			dma_handle->LIFCR|= (0x0000003D<<0);
		} else if(stream==DMA1_Stream1 || stream==DMA2_Stream1){
			dma_handle->LIFCR|= (0x0000003D<<6);
		}else if(stream==DMA1_Stream2 || stream==DMA2_Stream2){
			dma_handle->LIFCR|= (0x0000003D<<16);
		}else if(stream==DMA1_Stream3 || stream==DMA2_Stream3){
			dma_handle->LIFCR|= (0x0000003D<<22);
		}else if(stream==DMA1_Stream4 || stream==DMA2_Stream4){
			dma_handle->HIFCR|= (0x0000003D<<0);
		}else if(stream==DMA1_Stream5 || stream==DMA2_Stream5){
			dma_handle->HIFCR|= (0x0000003D<<6);
		}else if(stream==DMA1_Stream6 || stream==DMA2_Stream6){
			dma_handle->HIFCR|= (0x0000003D<<16);
		}else if(stream==DMA1_Stream7 || stream==DMA2_Stream7){
			dma_handle->HIFCR|= (0x0000003D<<22);
		}
		
		stream->M0AR= memBaseAdr;
		stream->NDTR= nod;
		
		stream->CR|=DMA_SxCR_EN;
	}
}
