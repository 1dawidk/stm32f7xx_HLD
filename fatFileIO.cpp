#include "fatFileIO.h"

uint16_t FATFileIO::open(FATFile *file, char* fileName, FATPartition *fatP){
	//Init file object
	file->name= fileName;
	file->dataClu=0;
	file->entryClu=0;
	file->entryOffset=0;
	file->onBuffSec=0;
	
	return fatP->FindFile(file);
}

uint16_t FATFileIO::read(FATFile *file, uint8_t *buff, uint32_t s, uint8_t len, FATPartition *fatP){
	uint16_t 	r;
	uint16_t 	fileSec;
	int8_t		buffId;
	uint8_t		*fileBuff;
	
	while(len>0){
		//Read sector
		fileSec= fatP->GetFirstCluSec(file->dataClu) + (s+len)/512;
		r=fatP->ReadFileSec(fileSec, &buffId);
		if(r!=FAT_OK)
			return r;
		fileBuff= fatP->GetFileBuff(buffId);
		
		//Copy bytes
		while( (((s+len)%512) + 1 != 512) && len>0 ){
			buff[len-1]= fileBuff[s+((len-1)%512)];
			len--;
		}
	}
	
	return FILEIO_OK;
}