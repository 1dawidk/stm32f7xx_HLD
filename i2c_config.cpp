#include "i2c_config.h"

I2CConfig::I2CConfig(){
	speed=I2C_SPEED_STANDARD;
	
	anfoff=I2C_AF_ON;
	dnf=I2C_DNF_DISABLE;
	
	addressLen= I2C_ADDRESSING_7_BIT;
	
	timeout=50;
}
