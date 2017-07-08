#ifndef _EXTI_H
#define _EXTI_H

#include "stm32f7xx.h"
#include "HighLayerDrivers/gpio.h"
#include "nvic.h"

#define EXTI_EDGE_RISING			0x01
#define EXTI_EDGE_FALLING			0x02

#define EXTI_LINE_0						0x00
#define EXTI_LINE_1						0x01
#define EXTI_LINE_2						0x02
#define EXTI_LINE_3						0x03
#define EXTI_LINE_4						0x04
#define EXTI_LINE_5						0x05
#define EXTI_LINE_6						0x06
#define EXTI_LINE_7						0x07
#define EXTI_LINE_8						0x08
#define EXTI_LINE_9						0x09
#define EXTI_LINE_10					0x0A
#define EXTI_LINE_11					0x0B
#define EXTI_LINE_12					0x0C
#define EXTI_LINE_13					0x0D
#define EXTI_LINE_14					0x0E
#define EXTI_LINE_15					0x0F

#define EXTI_SEGMENT_A				0x01
#define EXTI_SEGMENT_B				0x02
#define EXTI_SEGMENT_C				0x03
#define EXTI_SEGMENT_D				0x04
#define EXTI_SEGMENT_E				0x05
#define EXTI_SEGMENT_F				0x06
#define EXTI_SEGMENT_G				0x07
#define EXTI_SEGMENT_H				0x08
#define EXTI_SEGMENT_I				0x09
#define EXTI_SEGMENT_J				0x0A
#define EXTI_SEGMENT_K				0x0B


class Exti{
	public:
		static void ConfigLineInterrupt(GPIO_TypeDef *seg, uint16_t line, uint8_t edge, uint32_t preemptPriority, uint32_t subPriority);
		static void ConfigLineEvent(GPIO_TypeDef *seg, uint16_t pin, uint8_t edge);
		static uint8_t CheckIRQSergment(uint16_t line);
	
	private:
		static void SetIRQSeg(uint32_t seg, uint16_t line);
		static uint32_t TranslateGpioRegister(GPIO_TypeDef *seg);
};

#endif
