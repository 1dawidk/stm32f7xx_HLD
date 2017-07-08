#ifndef _HLD_PWR_H
#define _HLD_PWR_H

#include "stm32f7xx.h"
#include "HLDKernel.h"

#define PWR_OK										0
#define PWR_ERROR									1
#define PWR_TIMEOUT								2

#define PWR_VOLTAGE_SCALE1				PWR_CR1_VOS_1 | PWR_CR1_VOS_0
#define PWR_VOLTAGE_SCALE2				PWR_CR1_VOS_1
#define PWR_VOLTAGE_SCALE3				PWR_CR1_VOS_0

class Pwr{
	public:
		static uint8_t SetVoltageScaling(uint32_t scale);
		static uint8_t EnableOverDrive();
};

#endif
