#include "sdmmc.h"

SDMMC::SDMMC(SDMMC_TypeDef *sdmmc_h, DMA *dma_h){
	this->sdmmc_h= sdmmc_h;
	this->dma_h= dma_h;
}

void SDMMC::PrepareDMA(){
	DMAStreamConfig dmaStream;
	
	//Set DMA
	dmaStream.ch= DMA_CHANNEL_4;
	dmaStream.direction= DMA_DIRECTION_PER_MEM;
	dmaStream.periphAdr= (uint32_t)&(SDMMC1->FIFO);
	dmaStream.memDataSize= DMA_DATASIZE_WORD;
	dmaStream.periphDataSize= DMA_DATASIZE_WORD;
	dmaStream.memInc= DMA_INC_ENABLE;
	dmaStream.periphInc= DMA_INC_DISABLE;
	dmaStream.pburst=DMA_BURST_INCR4;
	dmaStream.mburst=DMA_BURST_INCR4;
	dmaStream.fc= DMA_FLOWCONTROL_ENABLE;
	dmaStream.priority= DMA_PRIORITY_VERY_HIGH;
	
	dma_h->registerStream(DMA2_Stream3, &dmaStream);
}

void SDMMC::WaitPCLK2(uint16_t n){
	n*=2;
	while(n--)
		__NOP;
}

void SDMMC::SetBusWidth(uint32_t width){
	sdmmc_h->CLKCR= (sdmmc_h->CLKCR&0xFFFFE7FF) | width;
}

void SDMMC::SetCK(uint8_t state){
	if(state)
		sdmmc_h->CLKCR|= SDMMC_CLKCR_CLKEN;
	else
		sdmmc_h->CLKCR&= ~SDMMC_CLKCR_CLKEN;
	WaitPCLK2(8);
}

void SDMMC::SetCKFreq(uint32_t sdmmcclkDiv){
	//Set new CLKDIV value and Clear BYPASS bit
	sdmmc_h->CLKCR= ((sdmmc_h->CLKCR & SDMMC_CLKCR_CLKDIV_MASK) | sdmmcclkDiv) & ~SDMMC_CLKCR_BYPASS;
	WaitPCLK2(8);
}

void SDMMC::SetMaxCKFreq(){
	//Set BYPASS bit
	sdmmc_h->CLKCR|= SDMMC_CLKCR_BYPASS;
	WaitPCLK2(8);
}

void SDMMC::SetPowerSupply(uint8_t state){
	if(state){
		sdmmc_h->POWER= 0x00000003;
	} else {
		sdmmc_h->POWER= 0x00000000;
	}
	WaitPCLK2(7);
}

void SDMMC::SetBlockSize(uint16_t size){
	sdmmc_h->DCTRL= (sdmmc_h->DCTRL&0xFFFFFF0F) | size<<4;
}

void SDMMC::SetDataLen(uint16_t bytes){
	sdmmc_h->DLEN= (sdmmc_h->DLEN&0xFE000000) | bytes;
}

void SDMMC::SetTimeout(uint32_t ticks){
	sdmmc_h->DTIMER= ticks;
}


uint32_t SDMMC::SendCmd(uint32_t cmd, uint32_t args, uint16_t respType){	
	//Check if CPSM is busy
	if(sdmmc_h->STA & SDMMC_CPSM_FLAG_CMDACT) return SDMMC_CPSM_BUSY;	
	
	//Clear Flags
	sdmmc_h->ICR|= SDMMC_CPSM_FLAG_CLEARALL;
	
	//Set Args
	sdmmc_h->ARG=args;
	//Set cmd index and resp type
	sdmmc_h->CMD=(0xFFFFF000&sdmmc_h->CMD) | (cmd&0x0000003F) | respType | SDMMC_CMD_CPSMEN;
	
	//Wait for cmd send finished
	while(sdmmc_h->STA & SDMMC_CPSM_FLAG_CMDACT);
	
	lastRespType=respType;
	
	//Return Flags
	return (sdmmc_h->STA & SDMMC_CPSM_FLAG_DONE);
}

uint8_t SDMMC::ReadCmdResponse(uint32_t *args){
	if(lastRespType==SDMMC_RESPTYPE_SHORT)
		args[0]= sdmmc_h->RESP1;
	else if(lastRespType==SDMMC_RESPTYPE_LONG){
		args[0]= sdmmc_h->RESP4;
		args[1]= sdmmc_h->RESP3;
		args[2]= sdmmc_h->RESP2;
		args[3]= sdmmc_h->RESP1;
	}
	
	return (uint8_t)(sdmmc_h->RESPCMD&(0x0000003F));
}

void SDMMC::PrepareToSend(uint8_t *data){
}

void SDMMC::PrepareToRead(uint32_t *buff){
	sdmmc_h->DCTRL&= ~SDMMC_DCTRL_DTEN;
	WaitPCLK2(10);
	
	//Setup DMA Stream
	dma_h->startStream(DMA2_Stream3, (uint32_t)buff, 0);
	
	//Card enabled to send, from card to controller, DMA enabled, block size 512 bytes
	sdmmc_h->DCTRL= SDMMC_DCTRL_DTEN | SDMMC_DCTRL_DTDIR |
									SDMMC_DCTRL_DMAEN | SDMMC_DCTRL_DBLOCKSIZE_3 | SDMMC_DCTRL_DBLOCKSIZE_0;
}

uint16_t SDMMC::IsReadFinished(){
	if( (sdmmc_h->STA & SDMMC_STA_DBCKEND) && !(sdmmc_h->STA & SDMMC_STA_RXACT) ){
		if( sdmmc_h->STA & (SDMMC_STA_RXOVERR | SDMMC_STA_DCRCFAIL))
			return SDMMC_EC_READ_ERROR;
		else
			return SDMMC_READ_FINISHED_OK;
	}else
		return SDMMC_READ_INPROGRESS;
}
