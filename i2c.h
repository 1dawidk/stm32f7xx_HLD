#ifndef _I2C_L_H
#define _I2C_L_H

#include "HLD/gpio.h"
#include "i2c_config.h"
#include "hldkernel.h"
#include "rcc.h"

class I2C{
	public:
		I2C(I2C_TypeDef *I2Cx);
		uint8_t Init(I2CConfig *i2cConfig);
		void 		Enable();
	
		uint8_t Transmit(uint8_t sla, uint8_t regAdr, uint8_t *buff, uint8_t size, uint8_t timeout);
		uint8_t Receive(uint8_t sla, uint8_t regAdr, uint8_t *buff, uint8_t size, uint8_t timeout);
	
	private:
		I2C_TypeDef *i2cHandle;
};

#endif

