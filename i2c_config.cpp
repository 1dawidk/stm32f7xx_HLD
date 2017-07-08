#include "i2c_config.h"

I2C_config::I2C_config(){
	speed=I2C_SPEED_STANDARD;
	
	anfoff=I2C_AF_ON;
	dnf=I2C_DNF_DISABLE;
	
	addressLen= I2C_ADDRESSING_7_BIT;
	
	timeout=50;
}
