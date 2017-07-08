#include "FATPartition.h"


FATPartition::FATPartition(SdCard *sdCard, uint32_t firstSectorLBA){
	this->sdCard= sdCard;
	this->firstSectorLBA= firstSectorLBA;
}

uint16_t FATPartition::Init(){
	uint8_t *data;
	uint16_t r;
	
	//Read first FAT sector
	r= sdCard->ReadBlock(firstSectorLBA, 0);
	//Check if read finished successfully
	if(r!=SDMMC_READ_FINISHED_OK)
		return r;
	
	//Get read data and find FAT sector
	data= (uint8_t*)sdCard->GetDataBuffer(0);
	
	return FAT_OK;
}

uint16_t FATPartition::IsValid(){
	if(firstSectorLBA>0 && fatStartSector>0 && dataStartSector>0)
		return FAT_OK;
	else
		return FAT_EC_UNKNOWN;
}

uint32_t FATPartition::GetFileStartSector(char* fileName){
	
}