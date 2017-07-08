#ifndef _LED_H
#define _LED_H

#include "stm32f7xx.h"
#include "HighLayerDrivers/gpio.h"
#include "HighLayerDrivers\hldkernel.h"

class Led{
	public:
		Led(uint16_t pin, GPIO_TypeDef *seg);
	
		void init();
	
		void on();
		void off();
		void toggle();
		void blink(uint8_t n, uint16_t delay);
		void set(uint8_t state);
	private:
		uint16_t pin;
		GPIO_TypeDef *seg;
};

#endif

