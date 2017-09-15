#include "dma.h"

DMA::DMA(DMA_TypeDef *dma_handle){
	this->dmaHandle= dma_handle;
}

void DMA::Init(){
	if(dmaHandle==DMA1){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_DMA1, ENABLE);
	} else if(dmaHandle==DMA2){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_DMA2, ENABLE);
	}
}

uint8_t DMA::RegisterStream(DMA_Stream_TypeDef *stream, DMAStreamConfig *dma_config){
	stream->CR&= ~DMA_SxCR_EN;
	
	//Set peripheral address
	stream->PAR= dma_config->periphAdr;
	//Set memory address
	stream->M0AR=dma_config->memoryAdr;
	
	
															
	//Set channel, priority, direction, burst
	stream->CR|= dma_config->ch | dma_config->priority | dma_config->direction;
	stream->CR|=  ((uint32_t)dma_config->memDataSize<<13) | ((uint32_t)dma_config->periphDataSize<<11);
	stream->CR|= ((uint32_t)dma_config->periphInc<<9) | ((uint32_t)dma_config->memInc<<10) | ((uint32_t)dma_config->circural<<8);
	stream->CR|= ((uint32_t)dma_config->mburst<<23) | ((uint32_t)dma_config->pburst<<21);
															
	//Set flow controller
	if(dma_config->fc)
		stream->CR|=DMA_SxCR_PFCTRL;
	
	//Set FIFO
	if(dma_config->mburst!=DMA_BURST_SINGLE || dma_config->pburst!=DMA_BURST_SINGLE){
		stream->FCR= DMA_SxFCR_DMDIS | DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1;
	}
	
	return 0;
}


//Clear flags, Set new Number of Data, Enable Stream
void DMA::StartStream(DMA_Stream_TypeDef *stream, uint16_t nod){
	if(!(stream->CR&DMA_SxCR_EN)){
		if(stream==DMA1_Stream0 || stream==DMA2_Stream0){
			dmaHandle->LIFCR|= (0x0000003D<<0);
		} else if(stream==DMA1_Stream1 || stream==DMA2_Stream1){
			dmaHandle->LIFCR|= (0x0000003D<<6);
		}else if(stream==DMA1_Stream2 || stream==DMA2_Stream2){
			dmaHandle->LIFCR|= (0x0000003D<<16);
		}else if(stream==DMA1_Stream3 || stream==DMA2_Stream3){
			dmaHandle->LIFCR|= (0x0000003D<<22);
		}else if(stream==DMA1_Stream4 || stream==DMA2_Stream4){
			dmaHandle->HIFCR|= (0x0000003D<<0);
		}else if(stream==DMA1_Stream5 || stream==DMA2_Stream5){
			dmaHandle->HIFCR|= (0x0000003D<<6);
		}else if(stream==DMA1_Stream6 || stream==DMA2_Stream6){
			dmaHandle->HIFCR|= (0x0000003D<<16);
		}else if(stream==DMA1_Stream7 || stream==DMA2_Stream7){
			dmaHandle->HIFCR|= (0x0000003D<<22);
		}
		
		stream->NDTR= nod;
		
		stream->CR|=DMA_SxCR_EN;
	}
}

void DMA::StartStream(DMA_Stream_TypeDef *stream, uint32_t mem_base_adr, uint16_t nod){
	if(!(stream->CR&DMA_SxCR_EN)){
		if(stream==DMA1_Stream0 || stream==DMA2_Stream0){
			dmaHandle->LIFCR|= (0x0000003D<<0);
		} else if(stream==DMA1_Stream1 || stream==DMA2_Stream1){
			dmaHandle->LIFCR|= (0x0000003D<<6);
		}else if(stream==DMA1_Stream2 || stream==DMA2_Stream2){
			dmaHandle->LIFCR|= (0x0000003D<<16);
		}else if(stream==DMA1_Stream3 || stream==DMA2_Stream3){
			dmaHandle->LIFCR|= (0x0000003D<<22);
		}else if(stream==DMA1_Stream4 || stream==DMA2_Stream4){
			dmaHandle->HIFCR|= (0x0000003D<<0);
		}else if(stream==DMA1_Stream5 || stream==DMA2_Stream5){
			dmaHandle->HIFCR|= (0x0000003D<<6);
		}else if(stream==DMA1_Stream6 || stream==DMA2_Stream6){
			dmaHandle->HIFCR|= (0x0000003D<<16);
		}else if(stream==DMA1_Stream7 || stream==DMA2_Stream7){
			dmaHandle->HIFCR|= (0x0000003D<<22);
		}
		
		stream->M0AR= mem_base_adr;
		stream->NDTR= nod;
		
		stream->CR|=DMA_SxCR_EN;
	}
}
