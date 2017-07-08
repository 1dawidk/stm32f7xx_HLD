#ifndef _MPU_REGION_CONFIG_H
#define _MPU_REGION_CONFIG_H

#include "stm32f7xx.h"

class MpuRegionConfig{
	public:
		uint32_t 	baseAddress;
		uint16_t 	size;
		uint8_t 	shareable;
		uint8_t 	number;
};

#endif
