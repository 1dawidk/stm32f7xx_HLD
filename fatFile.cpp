#include "fatFile.h"
#include "FATPartition.h"

FATFile::FATFile(){
	
}

FATFile::FATFile(char *name, FATPartition *fat_p, uint32_t data_clu, uint32_t entry_clu, uint16_t entry_offset, uint32_t size){
	this->name= 				name;
	this->fatP= 				fat_p;
	this->dataClu= 			data_clu;
	this->entryClu= 		entry_clu;
	this->size= 				size;
	this->entryOffset= 	entry_offset;
	this->atOffset=			0;
}

FATFile::~FATFile(){
	delete[] name;
}

void FATFile::operator<<(const char* str){
	//fatP->WriteFile(this, (uint8_t*)str, shift, length);
}

void FATFile::operator>>(char* buff){
	fatP->ReadFile(this, (uint8_t*)buff, 0, 10);
}

char* 		FATFile::GetName()				{return name;}
uint32_t 	FATFile::GetDataClu()			{return dataClu;}
uint32_t	FATFile::GetEntryClu()		{return entryClu;}
uint32_t 	FATFile::GetSize()				{return size;}
uint16_t 	FATFile::GetEntryOffset()	{return entryOffset;}
