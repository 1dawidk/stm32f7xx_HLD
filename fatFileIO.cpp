#include "fatFileIO.h"

uint16_t FATFileIO::open(FATFile *file, char* fileName, FATPartition *fatP){
	file->name= fileName;
	return fatP->FindFile(file);
}