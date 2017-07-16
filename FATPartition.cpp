#include "FATPartition.h"

/* PUBLIC METHODS */

FATPartition::FATPartition(SdCard *sdCard, uint32_t firstSectorLBA){
	this->sdCard= sdCard;
	this->firstSecLBA= firstSectorLBA;
	
	dataBuff= (uint8_t*)(new uint32_t[128]);
	fatBuff=	(uint8_t*)(new uint32_t[128]);
	for(uint8_t i=0; i<4; i++){
		filesBuff[i]= (uint8_t*)(new uint32_t[128]);
	}
	
	//Buffs empty and clean
	buffEmptyDirtyFlags=0xf0;
}

FATPartition::~FATPartition(){
	delete[] dataBuff;
	delete[] fatBuff;
	
	for(uint8_t i=0; i<4; i++)
		delete[] filesBuff[i];
}

uint16_t FATPartition::Init(){
	uint16_t BPB_RsvdSecCnt;
	uint32_t BPB_FATSz32;
	uint16_t r;
	
	//Read first FAT sector
	r= sdCard->ReadBlock(firstSecLBA, (uint32_t*)dataBuff);
	//Check if read finished successfully
	if(r!=SDMMC_READ_FINISHED_OK)
		return r;
	onDataBuffSec=0;
	
	//Check if partition is correct
	if(dataBuff[510]!=0x55 || dataBuff[511]!=0xAA)
		return FAT_EC_DAMAGED;
	
	secPerClu= dataBuff[FAT_BPB_SECPERCLUS_Offset];
	numFATs= dataBuff[FAT_BPB_NUMFATS_Offset];
	BPB_RsvdSecCnt= (uint16_t)dataBuff[FAT_BPB_RSVDSECCNT_Offset] |
										((uint16_t)dataBuff[FAT_BPB_RSVDSECCNT_Offset+1]<<8);
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

/* -- File handling -- */

uint16_t FATPartition::FindFile(FATFile *file){
	if(IsPathValid(file->name)==FAT_EC_INVALID_PATH)
		return FAT_EC_INVALID_PATH;

	char 			*path= file->name;
	uint8_t 	depth= GetPathDepth(path);
	uint8_t 	i=0;
	uint16_t 	r;
	uint32_t 	entryClu=2; //First data sector (NOT 0)
	uint32_t	dataClu=0;
	uint16_t	entryOffset;

	//Go through path
	while(i<depth){		
		//Search for next directory name and set up next sector	
		if(*path=='/')
			path++;
		
		r= FindFileDataClu(path, 1, &entryClu, &entryOffset, &entryClu);
		path= strchr(path, '/');
		
		if(r!=FAT_OK || path==0)
			return r;
		
		//Next level
		i++;
	}
	
	//Look for file in last directory
	if(*path=='/')
			path++;
	r=FindFileDataClu(path, 0, &entryClu, &entryOffset, &dataClu);
	if(r!=FAT_OK)
		return r;
	
	if(onDataBuffSec != GetFirstCluSec(entryClu) + entryOffset/512){
		r= sdCard->ReadBlock(GetFirstCluSec(entryClu)+firstSecLBA, (uint32_t*)dataBuff);
		if(r!=SDMMC_READ_FINISHED_OK)
			return r;
		onDataBuffSec=GetFirstCluSec(entryClu) + entryOffset/512;
	}
	
	file->dataClu= 			dataClu;
	file->entryClu= 		entryClu;
	file->entryOffset= 	entryOffset;
	file->size=					(uint32_t)dataBuff[entryOffset+28] | (uint32_t)dataBuff[entryOffset+29]<<8 | 
												(uint32_t)dataBuff[entryOffset+30]<<16 | (uint32_t)dataBuff[entryOffset+31]<<24;
	
	return FAT_OK;
}


uint16_t FATPartition::CreateFile(char *path, char *name, uint8_t type){
	return FAT_OK;
}


uint32_t FATPartition::GetFirstCluSec(uint32_t clu){
	return ((clu - 2)*secPerClu) + dataStartSec;
}

/* PRIVATE METHODS */

uint16_t FATPartition::FindFileDataClu(char *subPath, uint8_t dir, uint32_t *entryClu, uint16_t *offset, uint32_t *dataClu){
	uint16_t 	r;									//SD sector read response
	uint8_t 	found=0;						//Is file found
	uint8_t 	shortName=1;				//Is file name short
	uint8_t 	fileAttrs;					//File attributes
	uint8_t		sec;								//Current cluster sector
	
	if(*entryClu==2)
		*offset=0x20;
	else
		*offset=0x40;
	
	//Determine name type
	uint8_t dirNameLen;
	if(dir)
		dirNameLen= strchr(subPath, '/')-subPath;
	else
		dirNameLen= strlen(subPath);
	if(dirNameLen<=11){
		for(uint8_t i=0; i<dirNameLen; i++){
			if( *(subPath+i)!='.' && (*(subPath+i)<65 || *(subPath+i)>90)){
				shortName=0;
				break;
			}
		}
	} else {
		shortName=0;
	}
	
	//Read data sector, check if read finished successfully, get data buffer ptr
	if(onDataBuffSec!=GetFirstCluSec(*entryClu)){
		r= sdCard->ReadBlock(GetFirstCluSec(*entryClu)+firstSecLBA, (uint32_t*)dataBuff);
		if(r!=SDMMC_READ_FINISHED_OK)
			return r;
		onDataBuffSec=GetFirstCluSec(*entryClu);
	}
	
	//Search for name
	while(!found){
		//Get file entry attrs
		fileAttrs= dataBuff[*offset+0x0B];
		
		//Determine file entry type (long name, short name)
		if(fileAttrs==FAT_ATTR_LONG_NAME){
			if(shortName){
				//Inc search offset
				*offset+= 0x20 + 0x20*(dataBuff[*offset]&0x0F);
			} else {
				//Check
				r=CompareLongName(subPath, dir, entryClu, offset);
				if(!r){
					found=1;
					*offset+= 0x20*(dataBuff[*offset]&0x0F);
					*dataClu= (uint32_t)dataBuff[*offset+0x14]<<16 | (uint32_t)dataBuff[*offset+0x15]<<24 |
									(uint32_t)dataBuff[*offset+0x1A] | (uint32_t)dataBuff[*offset+0x1B]<<8;
					//Read and set file data cluster
				} else {
					*offset+= 0x20 + 0x20*(dataBuff[*offset]&0x0F);
				}
			}
		} else {
			if(shortName){
				//Check
				r=CompareShortName(subPath, dir, *offset);
				if(!r){
					found=1;
					*dataClu= (uint32_t)dataBuff[*offset+0x14]<<16 | (uint32_t)dataBuff[*offset+0x15]<<24 |
									(uint32_t)dataBuff[*offset+0x1A] | (uint32_t)dataBuff[*offset+0x1B]<<8;
					//Read and set file data cluster
				} else {
					*offset+= 0x20;
				}
			} else {
				//Inc search offset
				*offset+= 0x20;
			}
		}
		
		//if offset>512 look for next sector. If last than file not found
		sec= (*offset)/512;					//sector of cluster
		if(onDataBuffSec != GetFirstCluSec(*entryClu)+sec){
			if( sec < secPerClu){
				r= sdCard->ReadBlock(GetFirstCluSec(*entryClu)+sec+firstSecLBA, (uint32_t*)dataBuff);
				if(r!=SDMMC_READ_FINISHED_OK)
					return r;
				onDataBuffSec=GetFirstCluSec(*entryClu)+(sec);
			} else {
				r=GetNextCluster(*entryClu, entryClu);
				if(!r){
					*offset=0;
					sec=0;
					r= sdCard->ReadBlock(GetFirstCluSec(*entryClu)+sec+firstSecLBA, (uint32_t*)dataBuff);
					if(r!=SDMMC_READ_FINISHED_OK)
						return r;
					onDataBuffSec=GetFirstCluSec(*entryClu)+sec;
				} else if(r==FAT_EC_EOC)
					return FAT_EC_NOFILE;
				else
					return r;
			}
		}
	}
	
	//Determine if found or not
	if(found)
		return FAT_OK;
	else
		return FAT_EC_NOFILE;
}

uint16_t FATPartition::CompareShortName(char *subPath, uint8_t dir, uint16_t offset){
	uint8_t len;
	uint8_t l=0;
	if(dir)
		len= strchr(subPath, '/')-subPath;
	else
		len= strlen(subPath);
	
	for(uint8_t n=0; n<len; n++){
		if(*(subPath+n)=='.'){
			l=0x08;
			n++;
		}
		
		if( *(subPath+n)!=dataBuff[offset+l] )
			return FAT_EC_NOTTHESAME;
		
		l++;
	}
	
	return FAT_OK;
	
}

uint16_t FATPartition::CompareLongName(char *subPath, uint8_t dir, uint32_t *clu, uint16_t *offset){
	uint16_t r;
	uint8_t nameLen;
	uint8_t entryLen;
	uint8_t rem;
	uint8_t sec= (*offset)/512;
	
	if(dir)
		nameLen= strchr(subPath, '/')-subPath;
	else
		nameLen= strlen(subPath);
	
	entryLen= (nameLen/13);
	if(nameLen%13)
		entryLen++;
	
	if(entryLen!=(dataBuff[*offset]&0x0F))
		return FAT_EC_NOTTHESAME;
	
	while(nameLen--){			
		rem=nameLen%13;
		if(rem > 10){
			if(dataBuff[*offset + 2*rem + 6] != *(subPath+nameLen))
				return FAT_EC_NOTTHESAME;
		} else if(rem > 4){
			if(dataBuff[*offset + 2*rem + 4] != *(subPath+nameLen))
				return FAT_EC_NOTTHESAME;
		} else {
			if(dataBuff[*offset + 2*rem + 1] != *(subPath+nameLen))
				return FAT_EC_NOTTHESAME;
		}
		
		//Set proper offset
		if( (nameLen%13 == 0) && (nameLen/13) ){
			*offset+= 0x20;
			sec=(*offset)/512;
		}
		
		//Check if need to read next sector or cluster
		if(onDataBuffSec != GetFirstCluSec(*clu)+sec ){
			if( sec < secPerClu) {
				//Read next sector
				r= sdCard->ReadBlock(GetFirstCluSec(*clu)+sec+firstSecLBA, (uint32_t*)dataBuff);
				if(r!=SDMMC_READ_FINISHED_OK)
					return r;
				onDataBuffSec=GetFirstCluSec(*clu)+sec;
			} else {
				//Find and read next cluster
				GetNextCluster(*clu, clu);
				if(!r){
					*offset=0;
					sec=0;
					r= sdCard->ReadBlock(GetFirstCluSec(*clu)+sec+firstSecLBA, (uint32_t*)dataBuff);
					if(r!=SDMMC_READ_FINISHED_OK)
						return r;
					onDataBuffSec=GetFirstCluSec(*clu)+sec;
				} else if(r==FAT_EC_EOC)
					return FAT_EC_NOTTHESAME;
				else
					return r;
			}
		}
	}
	
	//Read short name entry sector
	if(onDataBuffSec != GetFirstCluSec(*clu)+sec ){
			if( sec < secPerClu) {
				//Read next sector
				r= sdCard->ReadBlock(GetFirstCluSec(*clu)+sec+firstSecLBA, (uint32_t*)dataBuff);
				if(r!=SDMMC_READ_FINISHED_OK)
					return r;
				onDataBuffSec=GetFirstCluSec(*clu)+sec;
			} else {
				GetNextCluster(*clu, clu);
				if(!r){
					*offset=0;
					sec=0;
					r= sdCard->ReadBlock(GetFirstCluSec(*clu)+sec+firstSecLBA, (uint32_t*)dataBuff);
					if(r!=SDMMC_READ_FINISHED_OK)
						return r;
					onDataBuffSec=GetFirstCluSec(*clu)+sec;
				} else if(r==FAT_EC_EOC)
					return FAT_EC_NOTTHESAME;
				else
					return r;
				//Find and read next cluster
			}
		}
	
	return FAT_OK;
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


uint16_t FATPartition::SetFileSize(FATFile *file){
	uint16_t r;
	
	if(onDataBuffSec != GetFirstCluSec(file->entryClu) + file->entryOffset/512){
		r= sdCard->ReadBlock(GetFirstCluSec(file->entryClu)+firstSecLBA, (uint32_t*)dataBuff);
		if(r!=SDMMC_READ_FINISHED_OK)
			return r;
		onDataBuffSec=GetFirstCluSec(file->entryClu) + file->entryOffset/512;
	}
}

uint8_t* FATPartition::GetFileBuff(int8_t idx){
	return filesBuff[idx];
}

uint16_t FATPartition::ReadFileSec(uint32_t sec, int8_t *buffId){
	*buffId= -1;
	uint16_t r;
	
	//Check if already on buff
	for(uint8_t i=0; i<4; i++){
		if(onFilesBuffSec[i]==sec){
			*buffId=i;
			return FAT_OK;
		}
	}
	
	//Look for empty buff
	if(buffEmptyDirtyFlags & 0x10)
			*buffId=0;
		else if(buffEmptyDirtyFlags & 0x20)
			*buffId=1;
		else if(buffEmptyDirtyFlags & 0x40)
			*buffId=2;
		else if(buffEmptyDirtyFlags & 0x80)
			*buffId=3;
	
	//Look for clean buff
	if(*buffId==-1){
		if(buffEmptyDirtyFlags & 0x01)
			*buffId=0;
		else if(buffEmptyDirtyFlags & 0x02)
			*buffId=1;
		else if(buffEmptyDirtyFlags & 0x04)
			*buffId=2;
		else if(buffEmptyDirtyFlags & 0x08)
			*buffId=3;
	}
	
	//If no free buffer flush one of them
	if( *buffId==-1 ){
		*buffId=0;
		//Flush buff
		sdCard->WriteBlock(onFilesBuffSec[*buffId]+firstSecLBA, (uint32_t*)filesBuff[*buffId]);
	}
	
	r= sdCard->ReadBlock(sec + firstSecLBA, (uint32_t*)filesBuff[*buffId]);
	if(r!=SDMMC_READ_FINISHED_OK)
		return r;
	onFilesBuffSec[*buffId]= sec;
	buffEmptyDirtyFlags|= 0x10<<(*buffId);
	
	
	return FAT_OK;
}



uint16_t FATPartition::WriteFileBytes(uint32_t sec, uint16_t offset, uint8_t *nBuff, uint16_t len){
	int8_t buffId;
	uint16_t r= ReadFileSec(sec, &buffId);
	
	if(buffId==-1){
		return FAT_EC_UNKNOWN;
	}	
	
	if(r){
		return r;
	}
	
	//Set dirty flag
	buffEmptyDirtyFlags|= 0x01<<buffId;
	
	//Write new buffer bytes
	while(len--){
		filesBuff[buffId][offset+len-1]= nBuff[len-1];
	}
	
	return FAT_OK;
}

/* Get next cluster
		clu- 			search for this cluster
		nextClu-	here will be writen next cluster

		RETURN:		Error code (including EOC) */
uint16_t	FATPartition::GetNextCluster(uint32_t clu, uint32_t *nextClu){
	uint32_t 	thisCluSector= 			fatStartSec+((clu*4)/512);
	uint32_t	thisCluSecOffset= 	(clu*4)%512;
	uint16_t	r;
	uint32_t	fatEntry;
	
	if(onFATBuffSec!=thisCluSector){
		r= sdCard->ReadBlock(thisCluSector+firstSecLBA, (uint32_t*)fatBuff);
		if(r!=SDMMC_READ_FINISHED_OK)
			return r;
		onFATBuffSec= thisCluSector;
	}
	
	fatEntry= (uint32_t)fatBuff[thisCluSecOffset] | (uint32_t)fatBuff[thisCluSecOffset+1]<<8 |
							(uint32_t)fatBuff[thisCluSecOffset+2]<<16 | (uint32_t)fatBuff[thisCluSecOffset+3]<<24;
	fatEntry&=0x0FFFFFFF;
	
	if( fatEntry == 0x0000001 )
		return FAT_EC_EOC;
	else if( fatEntry >= 0x0FFFFFF8 )
		return FAT_EC_EOC;
	else if( fatEntry >= 0x0FFFFFEF )
		return FAT_EC_BAD_SECTOR;
	else{
		*nextClu= fatEntry;
		return FAT_OK;
	}
}
