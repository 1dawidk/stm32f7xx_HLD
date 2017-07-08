#include "dma_stream_config.h"

DMAStreamConfig::DMAStreamConfig(){
		direction=0;
		periphAdr=0;
		memoryAdr=0;
		ch=0;
		mburst= DMA_BURST_SINGLE;
		pburst= DMA_BURST_SINGLE;
		priority=DMA_PRIORITY_LOW;
		fc=DMA_FLOWCONTROL_DISABLE;
		memDataSize=DMA_DATASIZE_BYTE;
		periphDataSize=DMA_DATASIZE_BYTE;
		periphInc=DMA_INC_DISABLE;
		memInc=DMA_INC_DISABLE;
		circural=DMA_CIRCULAR_DISABLE;
}
