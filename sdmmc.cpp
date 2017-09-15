#include "sdmmc.h"

SDMMC::SDMMC(SDMMC_TypeDef *sdmmc_h, DMA *dma_h){
	this->sdmmcHandle= sdmmc_h;
	this->dmaHandle= dma_h;
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
	
	dmaHandle->RegisterStream(DMA2_Stream3, &dmaStream);
	
	//Set DMA
	dmaStream.ch= DMA_CHANNEL_4;
	dmaStream.direction= DMA_DIRECTION_MEM_PER;
	dmaStream.periphAdr= (uint32_t)&(SDMMC1->FIFO);
	dmaStream.memDataSize= DMA_DATASIZE_WORD;
	dmaStream.periphDataSize= DMA_DATASIZE_WORD;
	dmaStream.memInc= DMA_INC_ENABLE;
	dmaStream.periphInc= DMA_INC_DISABLE;
	dmaStream.pburst=DMA_BURST_INCR4;
	dmaStream.mburst=DMA_BURST_INCR4;
	dmaStream.fc= DMA_FLOWCONTROL_ENABLE;
	dmaStream.priority= DMA_PRIORITY_VERY_HIGH;
	
	dmaHandle->RegisterStream(DMA2_Stream6, &dmaStream);
}

void SDMMC::WaitPCLK2(uint16_t n){
	n*=2;
	while(n--)
		__NOP;
}

void SDMMC::SetBusWidth(uint32_t width){
	sdmmcHandle->CLKCR= (sdmmcHandle->CLKCR&0xFFFFE7FF) | width;
}

void SDMMC::SetCK(uint8_t state){
	if(state)
		sdmmcHandle->CLKCR|= SDMMC_CLKCR_CLKEN;
	else
		sdmmcHandle->CLKCR&= ~SDMMC_CLKCR_CLKEN;
	WaitPCLK2(8);
}

void SDMMC::SetCKFreq(uint32_t sdmmc_clk_div){
	//Set new CLKDIV value and Clear BYPASS bit
	sdmmcHandle->CLKCR= ((sdmmcHandle->CLKCR & SDMMC_CLKCR_CLKDIV_MASK) | sdmmc_clk_div) & ~SDMMC_CLKCR_BYPASS;
	WaitPCLK2(8);
}

void SDMMC::SetMaxCKFreq(){
	//Set BYPASS bit
	sdmmcHandle->CLKCR|= SDMMC_CLKCR_BYPASS;
	WaitPCLK2(8);
}

void SDMMC::SetPowerSupply(uint8_t state){
	if(state){
		sdmmcHandle->POWER= 0x00000003;
	} else {
		sdmmcHandle->POWER= 0x00000000;
	}
	WaitPCLK2(7);
}

void SDMMC::SetBlockSize(uint16_t size){
	sdmmcHandle->DCTRL= (sdmmcHandle->DCTRL&0xFFFFFF0F) | size<<4;
}

void SDMMC::SetDataLen(uint16_t bytes){
	sdmmcHandle->DLEN= (sdmmcHandle->DLEN&0xFE000000) | bytes;
}

void SDMMC::SetTimeout(uint32_t ticks){
	sdmmcHandle->DTIMER= ticks;
}


uint32_t SDMMC::SendCmd(uint32_t cmd, uint32_t args, uint16_t respType){	
	//Check if CPSM is busy
	if(sdmmcHandle->STA & SDMMC_CPSM_FLAG_CMDACT) return SDMMC_CPSM_BUSY;	
	
	//Clear Flags
	sdmmcHandle->ICR|= SDMMC_CPSM_FLAG_CLEARALL;
	
	//Set Args
	sdmmcHandle->ARG=args;
	//Set cmd index and resp type
	sdmmcHandle->CMD=(0xFFFFF000&sdmmcHandle->CMD) | (cmd&0x0000003F) | respType | SDMMC_CMD_CPSMEN;
	
	//Wait for cmd send finished
	while(sdmmcHandle->STA & SDMMC_CPSM_FLAG_CMDACT);
	
	lastRespType=respType;
	
	//Return Flags
	return (sdmmcHandle->STA & SDMMC_CPSM_FLAG_DONE);
}

uint8_t SDMMC::ReadCmdResponse(uint32_t *args){
	if(lastRespType==SDMMC_RESPTYPE_SHORT)
		args[0]= sdmmcHandle->RESP1;
	else if(lastRespType==SDMMC_RESPTYPE_LONG){
		args[0]= sdmmcHandle->RESP4;
		args[1]= sdmmcHandle->RESP3;
		args[2]= sdmmcHandle->RESP2;
		args[3]= sdmmcHandle->RESP1;
	}
	
	return (uint8_t)(sdmmcHandle->RESPCMD&(0x0000003F));
}

void SDMMC::PrepareToRead(uint32_t *buff){
	sdmmcHandle->DCTRL&= ~SDMMC_DCTRL_DTEN;
	
	//Setup DMA Stream
	dmaHandle->StartStream(DMA2_Stream3, (uint32_t)buff, 0);
	
	//Clear flags
	sdmmcHandle->ICR|= 0x000005EF;
	
	//Card enabled to send, from card to controller, DMA enabled, block size 512 bytes
	sdmmcHandle->DCTRL= SDMMC_DCTRL_DTEN | SDMMC_DCTRL_DTDIR |
									SDMMC_DCTRL_DMAEN | SDMMC_DCTRL_DBLOCKSIZE_3 | SDMMC_DCTRL_DBLOCKSIZE_0;
}

uint16_t SDMMC::IsReadFinished(){
	if( (sdmmcHandle->STA & SDMMC_STA_DBCKEND) && !(sdmmcHandle->STA & SDMMC_STA_RXACT) ){
		if( sdmmcHandle->STA & (SDMMC_STA_RXOVERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT))
			return SDMMC_EC_READ_ERROR;
		else
			return SDMMC_READ_FINISHED_OK;
	}else
		return SDMMC_READ_INPROGRESS;
}

void SDMMC::PrepareToWrite(uint32_t *buff){
	sdmmcHandle->DCTRL&= ~SDMMC_DCTRL_DTEN;
	WaitPCLK2(10);
	
	//Clear cmd flags
	sdmmcHandle->ICR|= 0x000000C5;
	
	//Setup DMA Stream
	dmaHandle->StartStream(DMA2_Stream6, (uint32_t)buff, 0);
}

void SDMMC::StartWrite(){
	//Clear flags
	sdmmcHandle->ICR|= 0x000005DF;
	
	//Card enabled to send, from card to controller, DMA enabled, block size 512 bytes
	sdmmcHandle->DCTRL= SDMMC_DCTRL_DTEN | SDMMC_DCTRL_DMAEN | SDMMC_DCTRL_DBLOCKSIZE_3 | SDMMC_DCTRL_DBLOCKSIZE_0;
}

uint16_t SDMMC::IsWriteFinished(){
	if( (sdmmcHandle->STA & SDMMC_STA_DBCKEND) && !(sdmmcHandle->STA & SDMMC_STA_TXACT) ){
		if( sdmmcHandle->STA & (SDMMC_STA_TXUNDERR | SDMMC_STA_DCRCFAIL| SDMMC_STA_DTIMEOUT))
			return SDMMC_EC_WRITE_ERROR;
		else
			return SDMMC_WRITE_FINISHED_OK;
	}else
		return SDMMC_WRITE_INPROGRESS;
}
