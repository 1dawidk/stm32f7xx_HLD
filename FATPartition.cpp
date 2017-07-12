#include "FATPartition.h"

/*
		Every sector number with zeroSecLBA
*/

FATPartition::FATPartition(SdCard *sdCard, uint32_t firstSectorLBA){
	this->sdCard= sdCard;
	this->firstSecLBA= firstSectorLBA;
}

uint16_t FATPartition::Init(){
	uint16_t BPB_RsvdSecCnt;
	uint32_t BPB_FATSz32;
	uint16_t r;
	
	//Set buffer pointers
	fatBuff= (uint8_t*)sdCard->GetDataBuffer(0);
	dataBuff= (uint8_t*)sdCard->GetDataBuffer(1);
	
	//Read first FAT sector
	r= sdCard->ReadBlock(firstSecLBA, 1, HLD_DEVICECODE_FATPARTITION);
	//Check if read finished successfully
	if(r!=SDMMC_READ_FINISHED_OK)
		return r;
	onDataBuffSec=0;
	
	//Check if partition is correct
	if(dataBuff[510]!=0x55 || dataBuff[511]!=0xAA)
		return FAT_EC_DAMAGED;
	
	secPerClu= dataBuff[FAT_BPB_SECPERCLUS_Offset];
	numFATs= dataBuff[FAT_BPB_NUMFATS_Offset];
	bytsPerSec=			(uint16_t)dataBuff[FAT_BPB_BYTSPERSEC_Offset] |
										((uint16_t)dataBuff[FAT_BPB_BYTSPERSEC_Offset+1]<<8);
	BPB_RsvdSecCnt= (uint16_t)dataBuff[FAT_BPB_RSVDSECCNT_Offset] |
										((uint16_t)dataBuff[FAT_BPB_RSVDSECCNT_Offset+1]<<8);
	totDataSec= (uint32_t)dataBuff[FAT_BPB_TOTSEC32_Offset] |
									((uint32_t)dataBuff[FAT_BPB_TOTSEC32_Offset+1]<<8) |
									((uint32_t)dataBuff[FAT_BPB_TOTSEC32_Offset+2]<<16) |
									((uint32_t)dataBuff[FAT_BPB_TOTSEC32_Offset+3]<<24);
	BPB_FATSz32= (uint32_t)dataBuff[FAT_BPB_FATSZ32_Offset] |
									((uint32_t)dataBuff[FAT_BPB_FATSZ32_Offset+1]<<8) |
									((uint32_t)dataBuff[FAT_BPB_FATSZ32_Offset+2]<<16) |
									((uint32_t)dataBuff[FAT_BPB_FATSZ32_Offset+3]<<24);
	
	fatStartSec=		BPB_RsvdSecCnt;
	dataStartSec= 	BPB_RsvdSecCnt + (numFATs * BPB_FATSz32);
	
	onFATBuffSec=0xFFFFFFFF;
	onDataBuffSec=0xFFFFFFFF;
	
	return FAT_OK;
}

uint16_t FATPartition::IsValid(){
	if(firstSecLBA>0 && fatStartSec>0 && dataStartSec>0)
		return FAT_OK;
	else
		return FAT_EC_UNKNOWN;
}

uint32_t FATPartition::GetFirstCluSec(uint32_t clu){
	return ((clu - 2)*secPerClu) + dataStartSec;
}

uint16_t FATPartition::FindFile(FATFile *file){
	if(IsPathValid(file->name)==FAT_EC_INVALID_PATH)
		return FAT_EC_INVALID_PATH;

	char 			*path= file->name;
	uint8_t 	depth= GetPathDepth(path);
	uint8_t 	i=0;
	uint16_t 	r;
	uint32_t 	clu=GetFirstCluSec(2); //First data sector (NOT 0)
	
	return depth;
	
	/*
	//Go through path
	while(i<depth){		
		//Search for next directory name and set up next sector	
		if(*path=='/')
			path++;
		
		r= LookForFileCluster(path, 1, &clu);
		path= strchr(path, '/');
		
		if(r!=FAT_OK || path==0)
			return r;
		
		//Next level
		i++;
	}
	
	//Look for file in last directory
	LookForFileCluster(path, 0, &clu);
	
	return FAT_OK;*/
}

uint16_t FATPartition::LookForFileCluster(char *subPath, uint8_t dir, uint32_t *clu){
	uint8_t 	found=0;						//Is file found
	uint8_t 	shortName=1;				//Is file name short
	uint8_t 	fileAttrs;					//Attributes of file
	uint16_t 	r;									//SD sector read response
	uint16_t 	sOff;								//Search Offset
	uint8_t		cluSec=0;						//Sector of current cluster
	
	if(*clu==2)
		sOff=0x20;
	else
		sOff=0x40;
	
	//Determine name type
	uint8_t dirNameLen;
	if(dir)
		dirNameLen= strchr(subPath, '/')-subPath;
	else
		dirNameLen= strlen(subPath);
	if(dirNameLen<=11){
		for(uint8_t i=0; i<dirNameLen; i++){
			if( *(subPath+i)<65 || *(subPath+i)>90){
				shortName=0;
				break;
			}
		}
	} else {
		shortName=0;
	}
	
	//Read data sector, check if read finished successfully, get data buffer ptr
	r= sdCard->ReadBlock(GetFirstCluSec(*clu)+firstSecLBA, 1, HLD_DEVICECODE_FATPARTITION);
	if(r!=SDMMC_READ_FINISHED_OK)
		return r;
	onDataBuffSec=GetFirstCluSec(*clu);
	
	//Search for name
	while(!found){
		//Get file entry attrs
		fileAttrs= dataBuff[sOff+0x0B];
		
		//Determine file entry type (long name, short name)
		if(fileAttrs==FAT_ATTR_LONG_NAME){
			if(shortName){
				//Inc search offset
				sOff+= 0x20 + 0x20* (dataBuff[sOff]&0x0F);
			} else {
				//Check
				r=CompareLongName(subPath, dir, *clu, cluSec, sOff);
				if(!r){
					found=1;
					//Read and set file data cluster
				}
			}
		} else {
			if(shortName){
				//Check
				r=CompareShortName(subPath, dir, *clu, cluSec, sOff);
				if(!r){
					found=1;
					//Read and set file data cluster
				}
			} else {
				//Inc search offset
				sOff+= 0x20;
			}
		}
		
		//if offset>512 look for next sector. If last than file not found
		if(sOff>=512){
			cluSec++;
			
			if(cluSec<secPerClu){
				r= sdCard->ReadBlock(GetFirstCluSec(*clu)+cluSec+firstSecLBA, 1, HLD_DEVICECODE_FATPARTITION);
				if(r!=SDMMC_READ_FINISHED_OK)
					return r;
				onDataBuffSec=GetFirstCluSec(*clu)+cluSec;
			} else {
				r=GetNextCluster(*clu, clu);
				if(!r)
					cluSec=0;
				else if(r==FAT_EC_EOC)
					return FAT_EC_NOFILE;
				else
					return r;
			}
			
			sOff-=512;
		}
	}
	
	//Determine if found or not
	if(found)
		return FAT_OK;
	else
		return FAT_EC_NOFILE;
}

uint16_t FATPartition::CompareShortName(char *subPath, uint8_t dir, uint32_t clu, uint32_t sec, uint16_t offset){
	for(uint8_t i=0; i<11; i++){
		
	}
}

uint16_t FATPartition::CompareLongName(char *subPath, uint8_t dir, uint32_t clu, uint32_t sec, uint16_t offset){
	
}

uint8_t FATPartition::GetPathDepth(char *path){
	uint8_t level=0;
	
	while(*(++path)!=0){
		if(*path=='/')
			level++;
	}
	
	return level;
}

uint16_t FATPartition::IsPathValid(char *path){
	while(*(++path)!=0);
	
	if(*(path-1)=='/')
		return FAT_EC_INVALID_PATH;
	
	return FAT_OK;
}

/* Get next cluster
		clu- 			search for this cluster
		nextClu-	here will be writen next cluster

		RETURN:		Error code (including EOC) */
uint16_t	FATPartition::GetNextCluster(uint32_t clu, uint32_t *nextClu){
	uint32_t 	thisCluSector= 			fatStartSec+((clu*4)/bytsPerSec);
	uint32_t	thisCluSecOffset= 	(clu*4)%bytsPerSec;
	uint16_t	r;
	uint32_t	fatEntry;
	
	if(onFATBuffSec!=thisCluSector){
		r= sdCard->ReadBlock(thisCluSector+firstSecLBA, 0, HLD_DEVICECODE_FATPARTITION);
		if(r!=SDMMC_READ_FINISHED_OK)
			return r;
		onFATBuffSec= thisCluSector;
	}
	
	fatEntry= (uint32_t)fatBuff[thisCluSecOffset] | (uint32_t)fatBuff[thisCluSecOffset+1]<<8 |
							(uint32_t)fatBuff[thisCluSecOffset+2]<<16 | (uint32_t)fatBuff[thisCluSecOffset+3]<<24;
	fatEntry&=0x0FFFFFFF;
	
	if( fatEntry == 0x0000001 )
		return FAT_EC_EOC;
	else if( fatEntry > 0x0FFFFFF8 )
		return FAT_EC_EOC;
	else if( fatEntry > 0x0FFFFFEF )
		return FAT_EC_BAD_SECTOR;
	else{
		*nextClu= fatEntry;
		return FAT_OK;
	}
}
