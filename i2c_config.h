#ifndef _I2C_CONF_H
#define _I2C_CONF_H

#include "stm32f7xx.h"
#include "i2c_def.h"

class I2CConfig{
	public:
		GPIO_TypeDef *GPIOx;
		uint16_t sdaPin;
		uint16_t sclPin;
	
		uint8_t speed;
	
		uint8_t anfoff;
		uint8_t dnf;
	
		uint8_t addressLen;
	
		uint8_t timeout; // [ms]
	
		I2CConfig();
};

#endif
