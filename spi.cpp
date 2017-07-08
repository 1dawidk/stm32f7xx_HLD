#include "spi.h"

SPI::SPI(SPI_TypeDef *SPIx){
	this->spi_handle= SPIx;
}

uint8_t SPI::init(SpiConfig *spiConfig, GPIO_TypeDef *GPIOx, uint32_t miso_pin, uint32_t mosi_pin, uint32_t sck_pin, uint32_t cs_pin){
	//Set cs pin
	this->csSeg= GPIOx;
	this->csPin= cs_pin;
	
	//Init GPIO and enable SPI periph clock
	Gpio::InitAf(GPIOx, miso_pin, GPIO_PP, GPIO_AF5, GPIO_NOPULL);
	Gpio::InitAf(GPIOx, mosi_pin, GPIO_PP, GPIO_AF5, GPIO_NOPULL);
	Gpio::InitAf(GPIOx, sck_pin, GPIO_PP, GPIO_AF5, GPIO_NOPULL);
	
	if(spi_handle==SPI1){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_SPI1, RCC_ENABLE);
	}else if(spi_handle==SPI2) {
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_SPI2, RCC_ENABLE);
	}else if(spi_handle==SPI3){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_SPI3, RCC_ENABLE);
	}else if(spi_handle==SPI4){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_SPI4, RCC_ENABLE);
	}else if(spi_handle==SPI5){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_SPI5, RCC_ENABLE);
	}else if(spi_handle==SPI6){
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_SPI6, RCC_ENABLE);
	}
	
	uint32_t cr1Config=0x00;
	uint32_t cr2Config=0x00;
	
	//Set cs pin Hardware/Software management
	if(spiConfig->csHardwareCtrl){
		cr2Config|=SPI_CR2_SSOE;
	} else {
		cr1Config|=SPI_CR1_SSM;
	}
	
	//Set CPOL, CPHA, LSB first and MASTER config
	cr1Config|= ((uint32_t)spiConfig->CPOL<<1) | ((uint32_t)spiConfig->CPHA<<0) | ((uint32_t)spiConfig->lsbFirst<<7) | SPI_CR1_MSTR;
	//Set Data Size
	cr2Config|= ((uint32_t)spiConfig->dataLen<<8);
	//Set BaudRate
	cr1Config|= ((uint32_t)spiConfig->clkDivision<<3);
	
	//Set CR1 register
	spi_handle->CR1= cr1Config;
	//Set CR2 register
	spi_handle->CR2= cr2Config;
	
	//Enable SPI
	spi_handle->CR1 |= SPI_CR1_SPE;
	
	//Return state of SPI Enable Bit
	return !(spi_handle->CR1 & SPI_CR1_SPE);
}

uint8_t SPI::transmit(uint16_t dataOut, uint16_t timeout){
	//Read start Ticks
	uint32_t startTicks= HLDKernel::GetTimeMs();
	
	//Wait last transmition is finished
	while(spi_handle->SR&SPI_SR_BSY)
		if(startTicks+timeout<HLDKernel::GetTimeMs()) return 1;
	
	//Set new data to be transmited
	spi_handle->DR= dataOut;
	
	//Wait data transmition i finished
	while(spi_handle->SR & SPI_SR_BSY)
		if(startTicks+timeout<HLDKernel::GetTimeMs()) return 2;
	
	return 0;
}
	
uint8_t SPI::receive(uint16_t *dataIn, uint16_t timeout){
	//Read start Ticks
	uint32_t startTicks= HLDKernel::GetTimeMs();
	
	//Wait last transmition is finished
	while(spi_handle->SR&SPI_SR_BSY)
		if(startTicks+timeout<HLDKernel::GetTimeMs()) return 1;
	
	//Set 0x00 as data to start transmition
	spi_handle->DR= 0x00;
	
	//Wait data transmition finished
	while(spi_handle->SR & SPI_SR_BSY)
		if(startTicks+timeout<HLDKernel::GetTimeMs()) return 2;
	
	//Read received data
	(*dataIn)= spi_handle->DR;
	return 0;
}
	
uint8_t SPI::transreceive(uint16_t dataOut, uint16_t *dataIn, uint16_t timeout){
	//Read start Ticks
	uint32_t startTicks= HLDKernel::GetTimeMs();
	
	//Wait last transmition is finished
	while(spi_handle->SR&SPI_SR_BSY)
		if(startTicks+timeout<HLDKernel::GetTimeMs()) return 1;
	
	//Set data to be transmited
	spi_handle->DR= dataOut;
	
	//Wait data transmition finished
	while(spi_handle->SR & SPI_SR_BSY)
		if(startTicks+timeout<HLDKernel::GetTimeMs()) return 2;
	
	//Read received data
	(*dataIn)= spi_handle->DR;
	return 0;
}

void SPI::setCsPin(GPIO_TypeDef *GPIOx, uint32_t csPin){
	Gpio::InitOutput(GPIOx, csPin, GPIO_PP);
}
