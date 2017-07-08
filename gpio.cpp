#include "gpio.h"

void Gpio::InitInput(GPIO_TypeDef* GPIOx, uint8_t pin, uint16_t pp){
	GPIOx->MODER= (GPIOx->MODER&(~(0x00000003<<(pin*2)))) | (GPIO_MODE_INPUT<<(pin*2));
	GPIOx->OSPEEDR= (GPIOx->OSPEEDR&(~(0x00000003<<(pin*2)))) | (GPIO_SPEED_HIGH<<(pin*2));
	GPIOx->PUPDR= (GPIOx->PUPDR&(~(0x00000003<<(pin*2)))) | (pp<<(pin*2));
}

void Gpio::InitOutput(GPIO_TypeDef* GPIOx, uint8_t pin, uint16_t mode){
	GPIOx->MODER= 	(GPIOx->MODER&  (~(0x00000003<<(pin*2)))) | (GPIO_MODE_OUTPUT<<(pin*2));
	GPIOx->OSPEEDR= (GPIOx->OSPEEDR&(~(0x00000003<<(pin*2)))) | (GPIO_SPEED_HIGH<<(pin*2));
	GPIOx->OTYPER=  (GPIOx->OTYPER& (~(0x00000001<<pin)))     | (mode<<pin);
}

void Gpio::InitAf(GPIO_TypeDef* GPIOx, uint8_t pin, uint16_t mode, uint16_t af, uint16_t pull){
	if(pin<GPIO_PIN_8)
		GPIOx->AFR[0]= (GPIOx->AFR[0]&(~(0x0000000F<<(pin*4)))) | ((uint32_t)af<<(pin*4));
	else
		GPIOx->AFR[1]= (GPIOx->AFR[1]&(~(0x0000000F<<((pin-8)*4)))) | ((uint32_t)af<<((pin-8)*4));
	
	GPIOx->MODER= 	(GPIOx->MODER&  (~(0x00000003<<(pin*2)))) | (GPIO_MODE_ALTERNATE<<(pin*2));
	GPIOx->OSPEEDR= (GPIOx->OSPEEDR&(~(0x00000003<<(pin*2)))) | (GPIO_SPEED_VERYHIGH<<(pin*2));
	GPIOx->OTYPER=  (GPIOx->OTYPER& (~(0x00000001<<pin)))      |(mode<<pin);
	GPIOx->PUPDR= 	(GPIOx->PUPDR&  (~(0x00000003<<(pin*2)))) | (pull<<(pin*2));
}

void Gpio::InitAnalog(GPIO_TypeDef* GPIOx, uint8_t pin){
	GPIOx->OSPEEDR= (GPIOx->OSPEEDR&(~(0x00000003<<(pin*2)))) | (GPIO_SPEED_VERYHIGH<<(pin*2));
	GPIOx->MODER= 	(GPIOx->MODER&  (~(0x00000003<<(pin*2)))) | (GPIO_MODE_ANALOG<<(pin*2));
}

uint8_t Gpio::ReadInput(GPIO_TypeDef* GPIOx, uint8_t pin){
	return ((GPIOx->IDR & (1<<pin))>>pin);
}