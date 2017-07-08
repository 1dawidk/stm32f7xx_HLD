#ifndef _FATPARTITION_H
#define _FATPARTITION_H

#include "stm32f7xx.h"
#include "sdCard.h"

#define FAT_OK					0
#define FAT_EC_UNKNOWN	0x1581
#define FAT_EC_DAMAGED	0x1582

class FATPartition{
	public:
		FATPartition(SdCard *sdCard, uint32_t firstSectorLBA);
		uint16_t Init();
		uint16_t IsValid();
	
		uint32_t GetFileStartSector(char* fileName);
	private:
		SdCard *sdCard;
	
		uint32_t firstSectorLBA;
		uint32_t fatStartSector;
		uint32_t dataStartSector;
};

#endif
