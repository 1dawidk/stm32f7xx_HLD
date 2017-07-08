#ifndef _SPI_CONFIG_H
#define _SPI_CONFIG_H

#include "stm32f745xx.h"

class SpiConfig{
	public:
		uint16_t clkDivision;
		uint8_t lsbFirst;
		uint8_t dataLen;
		uint8_t CPOL;
		uint8_t CPHA;
		uint8_t csHardwareCtrl;
};

#endif
