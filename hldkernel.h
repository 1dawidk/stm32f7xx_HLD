#ifndef _HLD_KERNEL_H
#define _HLD_KERNEL_H

#include "stm32f7xx.h"

#define _ENABLE_				1
#define _DISABLE_				0

#define __NULL					(0x00)

/* HLD implements ErrorCodes
		
		Error Code is 16bit unsigned value.
		Error Code is a part of device implementation.
		It's definition looks like:
		[devic ename]_EC_[type: optional]_[Error spec]
		Eg: SDMMC_EC_TIMEOUT
		
		!!
				No Error is always ErrorCode= 0x0000
		!!
		
		Device can also implement Error Msg for Error Code. It is optional.
		If device implement Error Msg, EVERY Error Code should have Error Msg.
		Error Msg looks like:
		[devic ename]_EC_[type: optional]_[Error spec]_Msg
		Eg: SDMMC_EC_TIMEOUT_Msg
		
		Structure:
		
		Bit				Description
		15-8			Device code (User can define device code with id: 200-255)
		7					Type: Critical-1, Warning-0
		6-0				Error spec
		
		Devices Code:
		0-				Reserved 	|	10-	I2C						|	20-	SDMMC
		1-				Core			|	11-	RTC						|	21- FATPartition
		2-				Flash			|	12-	SPI						|	22- 
		3-				Rcc				|	13-	UART					|	23-
		4-				Pwr				|	14-	Periph Init		|	24-
		5-				NVIC			|	15-	SdIntf				|	25-
		6-				EXTI			|	16-	Random				|	26-
		7-				Mpu				|	17-	GPIO					|	27-
		8-				ADC				|	18-	FileIO				|	28-
		9-				DMA				|	19-	SdCard
*/

#define HLDKERNEL_DEVICECODE_CORE					0x01
#define HLDKERNEL_DEVICECODE_FLASH				0x02
#define HLDKERNEL_DEVICECODE_RCC					0x03
#define HLDKERNEL_DEVICECODE_PWR					0x04
#define HLDKERNEL_DEVICECODE_NVIC					0x05
#define HLDKERNEL_DEVICECODE_EXTI					0x06
#define HLDKERNEL_DEVICECODE_MPU					0x07
#define HLDKERNEL_DEVICECODE_ADC					0x08
#define HLDKERNEL_DEVICECODE_DMA					0x09
#define HLDKERNEL_DEVICECODE_I2C					0x0a
#define HLDKERNEL_DEVICECODE_RTC					0x0b
#define HLDKERNEL_DEVICECODE_SPI					0x0c
#define HLDKERNEL_DEVICECODE_UART					0x0d
#define HLDKERNEL_DEVICECODE_PERIPHINIT		0x0e
#define HLDKERNEL_DEVICECODE_SDINTF				0x0f
#define HLDKERNEL_DEVICECODE_RANDOM				0x10
#define HLDKERNEL_DEVICECODE_GPIO					0x11
#define HLDKERNEL_DEVICECODE_FILEIO				0x12
#define HLDKERNEL_DEVICECODE_SDCARD				0x13
#define HLDKERNEL_DEVICECODE_SDMMC				0x14
#define HLDKERNEL_DEVICECODE_FATPARTITION	0x15

class HLDKernel{
	public:
		static void delay_ms(uint32_t ms);
		static uint32_t GetTimeMs();
		static uint32_t GetStackSize();
		static void IncTimeMs();
		static uint32_t *ms;
	private:
		
	
		//Disable posibility creating object instance
		HLDKernel(){};
};

#endif
