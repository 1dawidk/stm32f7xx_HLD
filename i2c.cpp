#include "i2c.h"

#define I2C_GENERATE_START			i2c_handle->CR2 |= I2C_CR2_START
#define I2C_GENERATE_STOP				i2c_handle->CR2 |= I2C_CR2_STOP
#define I2C_GENERATE_ACK				i2c_handle->CR2 &= ~I2C_CR2_NACK
#define I2C_GENERATE_NACK				i2c_handle->CR2 |= I2C_CR2_ACK

I2C::I2C(I2C_TypeDef *I2Cx){
	i2c_handle=I2Cx;
}

uint8_t I2C::init(I2C_config *i2c_config){
	if(i2c_handle==I2C1){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_I2C1, RCC_ENABLE);
	} else if(i2c_handle==I2C2){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_I2C2, RCC_ENABLE);
	} else if(i2c_handle==I2C3){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_I2C3, RCC_ENABLE);
	} else if(i2c_handle==I2C4){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_I2C4, RCC_ENABLE);
	}
	
	Gpio::InitAf(i2c_config->GPIOx, i2c_config->sclPin, GPIO_OD, GPIO_AF4, GPIO_PULLUP);
	Gpio::InitAf(i2c_config->GPIOx, i2c_config->sdaPin, GPIO_OD, GPIO_AF4, GPIO_PULLUP);
	
	i2c_handle->CR1|= ((uint32_t)i2c_config->anfoff<<12) | ((uint32_t)i2c_config->dnf<<8);
	i2c_handle->CR2|= ((uint32_t)i2c_config->addressLen<<11);
	
	uint8_t presc;
	uint8_t scll;
	uint8_t sclh;
	uint8_t sdadel;
	uint8_t scldel;
	
	if(i2c_config->speed==I2C_SPEED_STANDARD){
		presc=I2C_STD_PRESC;
		scll=I2C_STD_SCLL;
		sclh=I2C_STD_SCLH;
		sdadel= I2C_STD_SDADEL;
		scldel= I2C_STD_SCLDEL;
	} else if(i2c_config->speed==I2C_SPEED_FAST){
		presc=I2C_FS_PRESC;
		scll=I2C_FS_SCLL;
		sclh=I2C_FS_SCLH;
		sdadel= I2C_FS_SDADEL;
		scldel= I2C_FS_SCLDEL;
	} else if(i2c_config->speed==I2C_SPEED_FAST_PLUS){
		presc=I2C_FSP_PRESC;
		scll=I2C_FSP_SCLL;
		sclh=I2C_FSP_SCLH;
		sdadel= I2C_FSP_SDADEL;
		scldel= I2C_FSP_SCLDEL;
	}

	
	i2c_handle->TIMINGR= ((uint32_t)presc<<28) | ((uint32_t)scldel<<20) | ((uint32_t)sdadel<<16) |((uint32_t)sclh<<8) | ((uint32_t)scll);
	
	return 0; 
}

void I2C::enable(){
	i2c_handle->CR1|= I2C_CR1_PE;
}

uint8_t I2C::transmit(uint8_t sla, uint8_t regAdr, uint8_t *buff, uint8_t size, uint8_t timeout){
	uint32_t ticktstart= HLDKernel::GetTimeMs();
	uint16_t cnt=0;
	
	//Clear All Flags
	i2c_handle->ICR=0x00003F38;
	
	//Set slave address, transfer direction
	i2c_handle->CR2= (i2c_handle->CR2 & 0xFFFFF800) | (sla&0xFE);
	//Set data length
	i2c_handle->CR2= (i2c_handle->CR2 & 0xFF00FFFF) | ((uint32_t)(size+1)<<16);
	//Set AUTO END mode
	i2c_handle->CR2|= I2C_CR2_AUTOEND;
	//Generate START + Slave Address + Data Direction
	I2C_GENERATE_START;
	
	//Wait for START, Slave Address send + ACK + Timeout
	while(i2c_handle->CR2 & I2C_CR2_START){
		if(ticktstart+timeout < HLDKernel::GetTimeMs()) return 1;
	}
	
	//Write TransmitRegister with register address
	i2c_handle->TXDR= regAdr;
	//Wait ,jgfor register address transfer complete + timeout
	while(!(i2c_handle->ISR & I2C_ISR_TXIS)){
		if(ticktstart+timeout < HLDKernel::GetTimeMs()) return 2;
	}
	
	while(cnt<size){
		//Write TransmitRegister
		i2c_handle->TXDR= buff[cnt];
		
		//Wait for last byte transfer complete + timeout
		while(!(i2c_handle->ISR & (I2C_ISR_TXIS|I2C_ISR_STOPF))){
			if(ticktstart+timeout < HLDKernel::GetTimeMs()) return cnt+3;
		}
		
		//Increment buffer cnt
		cnt++;
	}
	
	return 0;
}

uint8_t I2C::receive(uint8_t sla, uint8_t regAdr, uint8_t *buff, uint8_t size, uint8_t timeout){
	uint8_t cnt=0;
	uint32_t ticktstart= HLDKernel::GetTimeMs();
	
	//Clear All Flags
	i2c_handle->ICR=0x00003F38;
	
	/* Send Register Address */
	//Set slave address, transfer direction
	i2c_handle->CR2= (i2c_handle->CR2 & 0xFFFFF800) | sla;
	//Set data length
	i2c_handle->CR2= (i2c_handle->CR2 & 0xFF00FFFF) | 0x00010000;
	//Clear AUTO END mode
	i2c_handle->CR2&= ~I2C_CR2_AUTOEND;
	//Generate START + Slave Address + Data Direction
	I2C_GENERATE_START;
	//Wait for START, Slave Address send + ACK + Timeout
	while(!(i2c_handle->CR2 & I2C_CR2_START)){
		if(ticktstart+timeout < HLDKernel::GetTimeMs())
			return 15;
	}
	
	//Write TransmitRegister with register address
	i2c_handle->TXDR= regAdr;
	//Wait for register address transfer complete + timeout
	while(!(i2c_handle->ISR & I2C_ISR_TC)){
		if(ticktstart+timeout < HLDKernel::GetTimeMs())
			return 2;
	}
	
	//Clear All Flags
	i2c_handle->ICR=0x00003F38;
	
	/* Receive Register Data */
	//Set slave address, transfer direction
	i2c_handle->CR2= (i2c_handle->CR2 & 0xFFFFF800) | sla | I2C_CR2_RD_WRN;
	//Set data length
	i2c_handle->CR2= (i2c_handle->CR2 & 0xFF00FFFF) | ((uint32_t)size<<16);
	//Set AUTO END mode
	i2c_handle->CR2|= I2C_CR2_AUTOEND;
	//Generate START + Slave Address + Data Direction
	I2C_GENERATE_START;
	//Wait for reSTART,  Slave Address send + ACK + Timeout
	while(i2c_handle->CR2 & I2C_CR2_START){
		if(ticktstart+timeout < HLDKernel::GetTimeMs())
			return 3;
	}
	
	while(cnt<size){	
		//Wait for Receive register NOT empty
		while(!(i2c_handle->ISR & I2C_ISR_RXNE))
			if(ticktstart+timeout < HLDKernel::GetTimeMs())
				return cnt+4;
			
		buff[cnt]= i2c_handle->RXDR;
		
		//Increment cnt
		cnt++;
	}
	
	
	return 0;
}
