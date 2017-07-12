#ifndef FATFILE_H
#define FATFILE_H

#include "stm32f7xx.h"

typedef struct {
	char* name;
	uint8_t id;
	uint32_t firstClu;
	uint16_t cluSize;
	uint8_t onBuffSec;
} FATFile;

#endif
