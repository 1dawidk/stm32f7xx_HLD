#include "led.h"

Led::Led(uint16_t pin, GPIO_TypeDef *seg){
	this->pin= pin;
	this->seg= seg;
}

void Led::Init(){
	Gpio::InitOutput(seg,pin,GPIO_PP);
}
	
void Led::On(){
	GPIO_RESET_PIN(seg, pin);
}
void Led::Off(){
	GPIO_SET_PIN(seg, pin);
}
void Led::Toggle(){
	GPIO_TOG_PIN(seg, pin);
}

void Led::Blink(uint8_t n, uint16_t delay){
	Off();
	HLDKernel::delay_ms(delay);
	
	for(uint8_t i=0; i<n; i++){
		Toggle();
		HLDKernel::delay_ms(delay);
	}
}

void Led::Set(uint8_t state){
	if(state)
		GPIO_RESET_PIN(seg, pin);
	else
		GPIO_SET_PIN(seg, pin);
}
