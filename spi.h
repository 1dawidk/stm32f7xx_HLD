#ifndef _SPI_H
#define _SPI_H

#include "HighLayerDrivers/hldkernel.h"
#include "HighLayerDrivers/spi_config.h"
#include "HighLayerDrivers/rcc.h"
#include "HighLayerDrivers/gpio.h"

#define SPI_MODE_MASTER			0
#define SPI_MODE_SLAVE			1

#define SPI_CPOL_LOW				0
#define SPI_CPOL_HIGH				1

#define SPI_CPHA_FIRST			0
#define SPI_CPHA_SECOND			1

#define SPI_LSBFIRST				1
#define SPI_MSBFIRST				0

#define SPI_CLKDIV_2				0x0
#define SPI_CLKDIV_4				0x1
#define SPI_CLKDIV_8				0x2
#define SPI_CLKDIV_16				0x3
#define SPI_CLKDIV_32				0x4
#define SPI_CLKDIV_64				0x5
#define SPI_CLKDIV_128			0x6
#define SPI_CLKDIV_256			0x7

#define SPI_ENABLE					1
#define SPI_DISABLE					0


class SPI{
	public:
		SPI(SPI_TypeDef *SPIx);
		uint8_t init(SpiConfig *spiConfig, GPIO_TypeDef *GPIOx, uint32_t miso_pin, uint32_t mosi_pin, uint32_t sck_pin, uint32_t cs_pin);
		uint8_t transmit(uint16_t dataOut, uint16_t timeout);
		uint8_t receive(uint16_t *dataIn, uint16_t timeout);
		uint8_t transreceive(uint16_t dataOut, uint16_t *dataIn, uint16_t timeout);
		
		static void setCsPin(GPIO_TypeDef *GPIOx, uint32_t csPin);
	private:
		SPI_TypeDef *spi_handle;
	
		GPIO_TypeDef *csSeg;
		uint32_t csPin;
};

#endif
