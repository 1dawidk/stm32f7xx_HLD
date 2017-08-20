#ifndef FAT_FILE_IO_H
#define FAT_FILE_IO_H

#include "stm32f7xx.h"
#include "FATPartition.h"

#define FILEIO_OK 			0

class FATFileIO{
	public:
		static uint16_t open(FATFile *file, char* fileName, FATPartition *fatP);
		static uint16_t read(FATFile *file, uint8_t *buff, uint32_t s, uint8_t len, FATPartition *fatP);
		static uint16_t write(FATFile *file, uint8_t *buff, uint32_t s, uint16_t len, FATPartition *fatP);
		static uint16_t flush(FATFile *file);
};

#endif
