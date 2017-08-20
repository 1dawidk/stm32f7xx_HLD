#include "fatFile.h"
#include "FATPartition.h"

FATFile::FATFile(){
	
}

FATFile::FATFile(char *name, FATPartition *fatP, uint32_t dataClu, uint32_t entryClu, uint16_t entryOffset, uint32_t size){
	this->name= 				name;
	this->fatP= 				fatP;
	this->dataClu= 			dataClu;
	this->entryClu= 		entryClu;
	this->size= 				size;
	this->entryOffset= 	entryOffset;
	this->atOffset=			0;
}

FATFile::~FATFile(){
	delete[] name;
}

FATFile* FATFile::operator<<(const char* str){
	
}

FATFile* FATFile::operator>>(char* buff){
	
}

char* 		FATFile::GetName()				{return name;}
uint32_t 	FATFile::GetDataClu()			{return dataClu;}
uint32_t	FATFile::GetEntryClu()		{return entryClu;}
uint32_t 	FATFile::GetSize()				{return size;}
uint16_t 	FATFile::GetEntryOffset()	{return entryOffset;}
