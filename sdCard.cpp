#include "sdCard.h"
#include "sdintf.h"

SdCard::SdCard(SDMMC *sdmmc, UART *debugUart){
	this->sdmmc= sdmmc;
	this->debugUart= debugUart;
	state=State::Powered;
	this->RCA=0;
	csdKnown= SDCARD_CSD_UNKNOWN;
}

SdCard::~SdCard(){
}

uint16_t SdCard::Init(uint8_t dataBusWidth){
	uint32_t sdResponse[4];
	uint8_t transmitFlags;
	state= State::Ready;
	
	//Wait for stable voltage
	HLDKernel::delay_ms(100);
	
	//Broadcast CMD0, r=1 - Check voltage (Reset Card)
	transmitFlags=sdmmc->SendCmd(SDCARD_CMD0, 0, SDMMC_RESPTYPE_NORESP);
	HLDKernel::delay_ms(10);
		
	//Broadcast CMD8, r=1 - Check voltage
	transmitFlags=sdmmc->SendCmd(SDCARD_CMD8, SDCARD_CMD8_27_36, SDMMC_RESPTYPE_SHORT);
	if(transmitFlags & SDMMC_CPSM_FLAG_CMDREND){
		sdmmc->ReadCmdResponse(sdResponse);
		if(!(sdResponse[0]&SDCARD_CMD8_27_36)){
			return SDCARD_EC_WRONGVOLTAGE;
		}
	} else {
		return  SDCARD_EC_UNKNOWN;
	}
	
	//Prepare ACMD41 broadcast with r=SHORT, wait for Busy Bit for 20 repeats
	uint16_t i;
	for(i=0; i<20; i++){
		HLDKernel::delay_ms(10);
		
		sdmmc->SendCmd(SDCARD_CMD55, 0, SDMMC_RESPTYPE_NORESP);
		HLDKernel::delay_ms(5);
		transmitFlags=sdmmc->SendCmd(SDCARD_ACMD41, SDCARD_ACMD41_HCS | 0x00FF8000, SDMMC_RESPTYPE_SHORT);
		sdmmc->ReadCmdResponse(sdResponse);
		
		if(sdResponse[0]&0x80000000){
			break;
		}
	}
	//If not reaceived Busy Bit return ERROR
	if(i==20)
		return  SDCARD_EC_UNKNOWN;
	
	
	//Send CMD2, r=Long, arg=0
	HLDKernel::delay_ms(15);
	transmitFlags= sdmmc->SendCmd(SDCARD_CMD2, 0, SDMMC_RESPTYPE_LONG);
	if(!(transmitFlags&SDMMC_CPSM_FLAG_CMDREND)){
		return  SDCARD_EC_UNKNOWN;
	}
	
	//Send CMD3, r=Short, arg=0, Reaceive RCA
	HLDKernel::delay_ms(15);
	transmitFlags= sdmmc->SendCmd(SDCARD_CMD3, 0, SDMMC_RESPTYPE_SHORT);
	
	if(transmitFlags&SDMMC_CPSM_FLAG_CMDREND){
		sdmmc->ReadCmdResponse(sdResponse);
		this->RCA= (sdResponse[0]&0xFFFF0000)>>16;
	} else {
		return SDCARD_EC_UNKNOWN;
	}
	
	//Set 25Mhz ck freq
	HLDKernel::delay_ms(10);
	sdmmc->SetCKFreq(1);
	
	
	//Read CSD
	HLDKernel::delay_ms(10);
	transmitFlags= sdmmc->SendCmd(SDCARD_CMD9, (uint32_t)RCA<<16, SDMMC_RESPTYPE_LONG);
	
	if(transmitFlags&SDMMC_CPSM_FLAG_CMDREND){
		sdmmc->ReadCmdResponse(CSD);
		this->csdKnown=SDCARD_CSD_KNOWN;
	} else {
		this->csdKnown=SDCARD_CSD_UNKNOWN;
	}
	
	//Set data bus width
	if(dataBusWidth==SDINTF_BUSWIDTH_4){
		Select();
		HLDKernel::delay_ms(10);
		sdmmc->SendCmd(SDCARD_CMD55, (uint32_t)RCA<<16, SDMMC_RESPTYPE_NORESP);
		HLDKernel::delay_ms(1);
		transmitFlags= sdmmc->SendCmd(SDCARD_ACMD6, 0x00000002, SDMMC_RESPTYPE_SHORT);
		uint32_t resp;
		sdmmc->ReadCmdResponse(&resp);
		Select();
		
		if(transmitFlags&SDMMC_CPSM_FLAG_CMDREND){
			sdmmc->SetBusWidth(SDMMC_BUSWIDTH_4);
		} else {
			return SDCARD_EC_UNKNOWN;
		}
	}
	
	return SDCARD_OK;
}

/* READ BLOCK */
uint16_t SdCard::ReadBlock(uint32_t sector, uint8_t bufferId, uint32_t userId){
		Select();
		uint16_t readState;
		
		sdmmc->PrepareToRead(dataBuffer[bufferId]);
		uint32_t tf= sdmmc->SendCmd(SDCARD_CMD17, sector, SDMMC_RESPTYPE_SHORT);
		
		if(tf&SDMMC_CPSM_FLAG_CMDREND){
			while( (readState=sdmmc->IsReadFinished()) == SDMMC_READ_INPROGRESS);
			dataBufferUserId[bufferId]= userId;
			
			return readState;
		}
	
	return SDCARD_EC_UNKNOWN;	
}

/* READ N BLOCKS */
uint16_t SdCard::ReadBlocks(uint32_t sector, uint8_t bufferId, uint16_t n, uint32_t userId){
	if(Select()==SDCARD_OK){
		uint32_t tf= sdmmc->SendCmd(SDCARD_CMD18, sector, SDMMC_RESPTYPE_SHORT);
		
		
	}
	
	return SDCARD_EC_UNKNOWN;
}

/* WRITE BLOCK */
uint16_t SdCard::WriteBlock(uint32_t sector, uint8_t bufferId, uint32_t userId){
	if(Select()==SDCARD_OK){
		
	}
	
	return SDCARD_EC_UNKNOWN;
}

uint16_t SdCard::GetRCA(){
	return RCA;
}

void SdCard::LoadCSDRegister(){
	//Read CSD
	HLDKernel::delay_ms(10);
	uint32_t transmitFlags= sdmmc->SendCmd(SDCARD_CMD9, (uint32_t)RCA<<16, SDMMC_RESPTYPE_LONG);
	
	if(transmitFlags&SDMMC_CPSM_FLAG_CMDREND){
		sdmmc->ReadCmdResponse(CSD);
		this->csdKnown=SDCARD_CSD_KNOWN;
	}
}

uint16_t SdCard::Select(){
	uint32_t tf= sdmmc->SendCmd(SDCARD_CMD7, (uint32_t)RCA<<16, SDMMC_RESPTYPE_SHORT);
	
	if(tf&SDMMC_CPSM_FLAG_CMDREND){
		return SDCARD_OK;
	} else {
		return SDCARD_EC_SELECTERROR;
	}
}

void SdCard::Deselect(){
	uint32_t tf= sdmmc->SendCmd(SDCARD_CMD7, 0, SDMMC_RESPTYPE_NORESP);
}

/*
BLOCKNR = (C_SIZE+1) * MULT
MULT = 2C_SIZE_MULT+2 (C_SIZE_MULT < 8)
BLOCK_LEN = 2READ_BL_LEN, (READ_BL_LEN < 12)
*/

uint32_t SdCard::GetCapacity(){
	uint32_t size=0;
	
	if(csdKnown==SDCARD_CSD_KNOWN){
		uint32_t c;
		
		c= (CSD[1]&0xFFFF0000)>>16;
		c|= (CSD[2]&0x000003F)<<16;
		
		size= (c+1)/2;
	}
	
	return size;
}

uint16_t SdCard::GetMaxBusSpeed(){
	return CSD[0]&0x000000FF;
}

uint32_t* SdCard::GetDataBuffer(uint8_t id){
	return &(dataBuffer[id][0]);
}

uint32_t SdCard::GetBufferUserId(uint8_t id){
	return dataBufferUserId[id];
}

uint32_t* SdCard::GetCSD(){
	return CSD;
}