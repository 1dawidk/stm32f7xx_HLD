#ifndef _ADC_CHANNEL_H
#define _ADC_CHANNEL_H

//Sampling time [cycles]
#define ADC_CHANNEL_SMP_3			0x00
#define ADC_CHANNEL_SMP_15		0x01
#define ADC_CHANNEL_SMP_28		0x02
#define ADC_CHANNEL_SMP_56		0x03
#define ADC_CHANNEL_SMP_84		0x04
#define ADC_CHANNEL_SMP_112		0x05
#define ADC_CHANNEL_SMP_144		0x06
#define ADC_CHANNEL_SMP_480		0x07

#include "stm32f7xx.h"

class ADC_Channel{
	public:
		uint8_t no;
		uint8_t smp;
};

#endif
