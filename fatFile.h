#ifndef FATFILE_H
#define FATFILE_H

#include "stm32f7xx.h"

typedef struct {
	char* 		name;
	uint32_t 	dataClu;
	uint32_t 	entryClu;
	uint32_t 	size;
	uint16_t 	entryOffset;
	uint8_t		onBuffSec;
} FATFile;

#endif
