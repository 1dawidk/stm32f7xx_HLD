#ifndef _SDMMC_CONFIG_H
#define _SDMMC_CONFIG_H

#include "stm32f7xx.h"

class SDConfig{
	public:
		//SD Interface pinout
		GPIO_TypeDef *ckSeg;
		GPIO_TypeDef *dataSeg;
		GPIO_TypeDef *cmdSeg;
		GPIO_TypeDef *senseSeg;	
		uint16_t ckPin;
		uint16_t d0Pin;
		uint16_t d1Pin;
		uint16_t d2Pin;
		uint16_t d3Pin;
		uint16_t cmdPin;
		//SD Card sensing
		uint16_t sensePin;
		uint8_t useSenseEXTI;
		uint8_t senseEXTIProirity;
		uint8_t senseEXTISubPriority;
		//Data transfer
		uint8_t dataBusWidth;
};

#endif
