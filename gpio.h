#ifndef _GPIO_H
#define _GPIO_H

#include <stm32f7xx.h>

#define GPIO_SET_PIN(x, y)					(x->ODR |= 1<<y)
#define GPIO_RESET_PIN(x, y)				(x->ODR &= ~(1<<y))
#define GPIO_TOG_PIN(x, y)					(x->ODR ^= 1<<y)
#define GPIO_READ_PIN(x, y)					(x->IDR & (1<<y))

#define GPIO_PIN_0                 	0x00
#define GPIO_PIN_1                 	0x01
#define GPIO_PIN_2                 	0x02
#define GPIO_PIN_3                 	0x03
#define GPIO_PIN_4                 	0x04
#define GPIO_PIN_5                 	0x05
#define GPIO_PIN_6                 	0x06
#define GPIO_PIN_7                 	0x07
#define GPIO_PIN_8                 	0x08
#define GPIO_PIN_9                 	0x09
#define GPIO_PIN_10                	0x0A
#define GPIO_PIN_11                	0x0B
#define GPIO_PIN_12                	0x0C
#define GPIO_PIN_13                	0x0D
#define GPIO_PIN_14                	0x0E
#define GPIO_PIN_15                	0x0F

#define GPIO_MODE_INPUT							((uint32_t)0x00)
#define GPIO_MODE_OUTPUT						((uint32_t)0x01)
#define GPIO_MODE_ALTERNATE					((uint32_t)0x02)
#define GPIO_MODE_ANALOG						((uint32_t)0x03)

#define GPIO_PP											0
#define GPIO_OD 										1

#define GPIO_SPEED_LOW							0x00
#define GPIO_SPEED_MEDIUM						0x01
#define GPIO_SPEED_HIGH							0x02
#define GPIO_SPEED_VERYHIGH					0x03

#define GPIO_NOPULL									0x00
#define GPIO_PULLUP									0x01
#define GPIO_PULLDOWN								0x02

#define GPIO_AF0										0x00
#define GPIO_AF1										0x01
#define GPIO_AF2										0x02
#define GPIO_AF3										0x03
#define GPIO_AF4										0x04
#define GPIO_AF5										0x05
#define GPIO_AF6										0x06
#define GPIO_AF7										0x07
#define GPIO_AF8										0x08
#define GPIO_AF9										0x09
#define GPIO_AF10										0x0A
#define GPIO_AF11										0x0B
#define GPIO_AF12										0x0C
#define GPIO_AF13										0x0D
#define GPIO_AF14										0x0E
#define GPIO_AF15										0x0F

class Gpio{
	public:
		static void InitInput(GPIO_TypeDef* GPIOx, uint8_t pin, uint16_t pp);
		static void InitOutput(GPIO_TypeDef* GPIOx, uint8_t pin, uint16_t mode);
		static void InitAf(GPIO_TypeDef* GPIOx, uint8_t pin, uint16_t mode, uint16_t af, uint16_t pull);
		static void InitAnalog(GPIO_TypeDef* GPIOx, uint8_t pin);
		static uint8_t ReadInput(GPIO_TypeDef* GPIOx, uint8_t pin);
};

#endif
