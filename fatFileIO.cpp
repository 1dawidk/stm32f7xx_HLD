#include "fatFileIO.h"

uint16_t FATFileIO::open(FATFile *file, char* fileName, FATPartition *fatP){
	//Init file object
	file->name= fileName;
	file->firstClu=0;
	file->id=HLDKernel::GetTimeMs();
	file->cluSize=0;
	file->onBuffSec=0;
	
	return fatP->FindFile(file);
}