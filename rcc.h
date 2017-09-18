#ifndef _HLD_RCC_H
#define _HLD_RCC_H

#include "stm32f7xx.h"
#include "hldkernel.h"

#define RCC_ENABLE						1
#define RCC_DISABLE						0

#define RCC_OK								0
#define RCC_ERROR							1
#define RCC_TIMEOUT						2

#define RCC_PLLSOURCE_HSI     RCC_PLLCFGR_PLLSRC_HSI
#define RCC_PLLSOURCE_HSE     RCC_PLLCFGR_PLLSRC_HSE

#define RCC_PLLP_DIV2         ((uint32_t)0x00000000U)
#define RCC_PLLP_DIV4         ((uint32_t)0x00000001U)
#define RCC_PLLP_DIV6         ((uint32_t)0x00000002U)
#define RCC_PLLP_DIV8         ((uint32_t)0x00000003U)

#define RCC_OSCILLATOR_HSI		0
#define RCC_OSCILLATOR_HSE		1
#define RCC_OSCILLATOR_LSI		2
#define RCC_OSCILLATOR_LSE		3

#define RCC_BUS_TYPE_MASK					0xE0
#define RCC_PERIPHCLK_SHIFT_MASK	0x1F
#define RCC_BUS_AHB1							0x20
#define RCC_BUS_AHB2							0x40
#define RCC_BUS_AHB3							0x60
#define RCC_BUS_APB1							0x80
#define RCC_BUS_APB2							0xA0

//AHB1 Clk EN
#define RCC_PERIPHCLK_OTG_HSULPI	0x3E
#define RCC_PERIPHCLK_OTG_HS			0x3D
#define RCC_PERIPHCLK_ETHMAC_PTP	0x3C
#define RCC_PERIPHCLK_ETHMAC_RX		0x3B
#define RCC_PERIPHCLK_ETHMAC_TX		0x3A
#define RCC_PERIPHCLK_ETHMAC			0x39
#define RCC_PERIPHCLK_DMA2D				0x37
#define RCC_PERIPHCLK_DMA2				0x36
#define RCC_PERIPHCLK_DMA1				0x35
#define RCC_PERIPHCLK_DTCM_RAM		0x34
#define RCC_PERIPHCLK_BKP_SRAM		0x32
#define RCC_PERIPHCLK_CRC					0x2C
#define RCC_PERIPHCLK_GPIOK				0x2A
#define RCC_PERIPHCLK_GPIOJ				0x29
#define RCC_PERIPHCLK_GPIOI				0x28
#define RCC_PERIPHCLK_GPIOH				0x27
#define RCC_PERIPHCLK_GPIOG				0x26
#define RCC_PERIPHCLK_GPIOF				0x25
#define RCC_PERIPHCLK_GPIOE				0x24
#define RCC_PERIPHCLK_GPIOD				0x23
#define RCC_PERIPHCLK_GPIOC				0x22
#define RCC_PERIPHCLK_GPIOB				0x21
#define RCC_PERIPHCLK_GPIOA				0x20

//AHB2 Clk EN
#define RCC_PERIPHCLK_OTG_FS			0x47
#define RCC_PERIPHCLK_RNG					0x46
#define RCC_PERIPHCLK_HASH				0x45
#define RCC_PERIPHCLK_CRYP				0x44
#define RCC_PERIPHCLK_DCMI				0x40

//AHB3 Clk EN
#define RCC_PERIPHCLK_QSPI				0x61
#define RCC_PERIPHCLK_FMC					0x60

//APB1 Clk EN
#define RCC_PERIPHCLK_UART8				0x9F
#define RCC_PERIPHCLK_UART7				0x9E
#define RCC_PERIPHCLK_DAC					0x9D
#define RCC_PERIPHCLK_PWR					0x9C
#define RCC_PERIPHCLK_CEC					0x9B
#define RCC_PERIPHCLK_CAN2				0x9A
#define RCC_PERIPHCLK_CAN1				0x99
#define RCC_PERIPHCLK_I2C4				0x98
#define RCC_PERIPHCLK_I2C3				0x97
#define RCC_PERIPHCLK_I2C2				0x96
#define RCC_PERIPHCLK_I2C1				0x95
#define RCC_PERIPHCLK_UART5				0x94
#define RCC_PERIPHCLK_UART4				0x93
#define RCC_PERIPHCLK_USART3			0x92
#define RCC_PERIPHCLK_USART2			0x91
#define RCC_PERIPHCLK_SPDIFRX			0x90
#define RCC_PERIPHCLK_SPI3				0x8F
#define RCC_PERIPHCLK_SPI2				0x8E
#define RCC_PERIPHCLK_WWDG				0x8B
#define RCC_PERIPHCLK_LPTMI1			0x89
#define RCC_PERIPHCLK_TIM14				0x88
#define RCC_PERIPHCLK_TIM13				0x87
#define RCC_PERIPHCLK_TIM12				0x86
#define RCC_PERIPHCLK_TIM7				0x85
#define RCC_PERIPHCLK_TIM6				0x84
#define RCC_PERIPHCLK_TIM5				0x83
#define RCC_PERIPHCLK_TIM4				0x82
#define RCC_PERIPHCLK_TIM3				0x81
#define RCC_PERIPHCLK_TIM2				0x80

//APB2 Clk EN
#define RCC_PERIPHCLK_DSI					0xBB
#define RCC_PERIPHCLK_LTDC				0xBA	
#define RCC_PERIPHCLK_SAI2				0xB7
#define RCC_PERIPHCLK_SAI1				0xB6
#define RCC_PERIPHCLK_SPI6				0xB5
#define RCC_PERIPHCLK_SPI5				0xB4
#define RCC_PERIPHCLK_TIM11				0xB2
#define RCC_PERIPHCLK_TIM10				0xB1
#define RCC_PERIPHCLK_TIM9				0xB0
#define RCC_PERIPHCLK_SYSCFG			0xAE
#define RCC_PERIPHCLK_SPI4				0xAD
#define RCC_PERIPHCLK_SPI1				0xAC
#define RCC_PERIPHCLK_SDMMC1			0xAB
#define RCC_PERIPHCLK_ADC3				0xAA
#define RCC_PERIPHCLK_ADC2				0xA9
#define RCC_PERIPHCLK_ADC1				0xA8
#define RCC_PERIPHCLK_USART6			0xA5
#define RCC_PERIPHCLK_USART1			0xA4
#define RCC_PERIPHCLK_TIM8				0xA1
#define RCC_PERIPHCLK_TIM1				0xA0

#define RCC_AHB_CLKDIV_1					0x0
#define RCC_AHB_CLKDIV_2					0x8
#define RCC_AHB_CLKDIV_4					0x9
#define RCC_AHB_CLKDIV_8					0xA
#define RCC_AHB_CLKDIV_16					0xB
#define RCC_AHB_CLKDIV_64					0xC
#define RCC_AHB_CLKDIV_128				0xD
#define RCC_AHB_CLKDIV_256				0xE
#define RCC_AHB_CLKDIV_512				0xF

#define RCC_APB_CLKDIV_1					0x0
#define RCC_APB_CLKDIV_2					0x4
#define RCC_APB_CLKDIV_4					0x5
#define RCC_APB_CLKDIV_8					0x6
#define RCC_APB_CLKDIV_16					0x7

#define RCC_SYSCLK_SOURCE_HSI			0x0
#define RCC_SYSCLK_SOURCE_HSE			0x1
#define RCC_SYSCLK_SOURCE_PLL			0x2

#define RCC_48MHZCLK_SOURCE_PLL		0x0


//Dedic Clk source 1
#define RCC_DEDICCLK_TIMPRE					0x0018
#define RCC_DEDICCLK_TIMPRE_0				0x00
#define RCC_DEDICCLK_TIMPRE_1				0x01

#define RCC_DEDICCLK_SAI2						0x0016
#define RCC_DEDICCLK_SAI1						0x14
#define RCC_DEDICCLK_SAI_PLLSAI			0x00
#define RCC_DEDICCLK_SAI_PLLI2S			0x01
#define RCC_DEDICCLK_SAI_ALT				0x02

#define RCC_DEDICCLK_PLLSAIDIVR			0x0010
#define RCC_DEDICCLK_PLLSAIDIVR_2		0x00
#define RCC_DEDICCLK_PLLSAIDIVR_4		0x01
#define RCC_DEDICCLK_PLLSAIDIVR_8		0x02
#define RCC_DEDICCLK_PLLSAIDIVR_16	0x03

#define RCC_DEDICCLK_PLLSAIDIVQ			0x0008
#define RCC_DEDICCLK_PLLI2SDIVQ			0x0000

//Dedic Clk source 2
#define RCC_DEDICCLK_SDMMC1					0x011C
#define RCC_DEDICCLK_SDMMC1_CK48		0x00
#define RCC_DEDICCLK_SDMMC1_HCLK		0x01

#define RCC_DEDICCLK_CK48M					0x011B
#define RCC_DEDICCLK_CK48M_PLL			0x00
#define RCC_DEDICCLK_CK48M_PLLSAI		0x01

#define RCC_DEDICCLK_CEC						0x011A
#define RCC_DEDICCLK_CEC_LSE				0x00
#define RCC_DEDICCLK_CEC_HSID488		0x01

#define RCC_DEDICCLK_LPTIM1					0x0118
#define RCC_DEDICCLK_LPTIM1_PCLK1		0x00
#define RCC_DEDICCLK_LPTIM1_LSI			0x01
#define RCC_DEDICCLK_LPTIM1_HSI			0x02
#define RCC_DEDICCLK_LPTIM1_LSE			0x03

#define RCC_DEDICCLK_I2C4						0x0116
#define RCC_DEDICCLK_I2C3						0x0114
#define RCC_DEDICCLK_I2C2						0x0112
#define RCC_DEDICCLK_I2C1						0x0110
#define RCC_DEDICCLK_I2C_PCLK1			0x00
#define RCC_DEDICCLK_I2C_HCLK				0x01
#define RCC_DEDICCLK_I2C_HSI				0x02

#define RCC_DEDICCLK_UART8					0x010E
#define RCC_DEDICCLK_UART7					0x010C
#define RCC_DEDICCLK_USART6					0x010A
#define RCC_DEDICCLK_UART5					0x0108
#define RCC_DEDICCLK_UART4					0x0106
#define RCC_DEDICCLK_USART3					0x0104
#define RCC_DEDICCLK_USART2					0x0102
#define RCC_DEDICCLK_USART1					0x0100
#define RCC_DEDICCLK_USART_PCLK			0x00
#define RCC_DEDICCLK_USART_HCLK			0x01
#define RCC_DEDICCLK_USART_HSI			0x02
#define RCC_DEDICCLK_USART_LSE			0x03
#define RCC_DEDICCLK_UART_PCLK			0x00
#define RCC_DEDICCLK_UART_HCLK			0x01
#define RCC_DEDICCLK_UART_HSI				0x02
#define RCC_DEDICCLK_UART_LSE				0x03

class Core;

class Rcc{
	public:
		static void Deinit();
	
		static uint8_t SetPLL(uint32_t source, uint32_t pllm, uint32_t plln, uint32_t pllp, uint32_t pllq);
		static uint8_t TurnOnOsc(uint8_t osc);
		static uint8_t TurnOffOsc(uint8_t osc);
	
		static uint8_t SetAHBDivider(uint32_t divider);
		static uint8_t SetAPB1Divider(uint32_t divider);
		static uint8_t SetAPB2Divider(uint32_t divider);
	
		static uint8_t SetSystemClkSource(uint32_t source);
	
		static void SetPeriphClkState(uint32_t periph, uint8_t state);
		static void SetDedicatedClkSrc(uint16_t periph, uint8_t source);
};

#endif
