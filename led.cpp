#include "led.h"

Led::Led(uint16_t pin, GPIO_TypeDef *seg){
	this->pin= pin;
	this->seg= seg;
}

void Led::init(){
	Gpio::InitOutput(seg,pin,GPIO_PP);
}
	
void Led::on(){
	GPIO_RESET_PIN(seg, pin);
}
void Led::off(){
	GPIO_SET_PIN(seg, pin);
}
void Led::toggle(){
	GPIO_TOG_PIN(seg, pin);
}

void Led::blink(uint8_t n, uint16_t delay){
	off();
	HLDKernel::delay_ms(delay);
	
	for(uint8_t i=0; i<n; i++){
		toggle();
		HLDKernel::delay_ms(delay);
	}
}

void Led::set(uint8_t state){
	if(state)
		GPIO_RESET_PIN(seg, pin);
	else
		GPIO_SET_PIN(seg, pin);
}
