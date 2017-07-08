#ifndef _I2C_L_H
#define _I2C_L_H

#include "HighLayerDrivers/gpio.h"
#include "i2c_config.h"
#include "hldkernel.h"
#include "rcc.h"

class I2C{
	public:
		I2C(I2C_TypeDef *I2Cx);
		uint8_t init(I2C_config *i2c_config);
		void enable();
	
		uint8_t transmit(uint8_t sla, uint8_t regAdr, uint8_t *buff, uint8_t size, uint8_t timeout);
		uint8_t receive(uint8_t sla, uint8_t regAdr, uint8_t *buff, uint8_t size, uint8_t timeout);
	
	private:
		I2C_TypeDef *i2c_handle;
};

#endif

