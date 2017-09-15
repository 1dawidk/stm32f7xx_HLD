#include "random.h"

uint8_t Random::Init(){
	locker= uosLocker_Create();
	
	//Init clock source
	Rcc::SetPeriphClkState(RCC_PERIPHCLK_RNG, RCC_ENABLE);
	
	//Clear status register
	RNG->SR= 0x00000000;
	
	//Turn on random number generator
	RNG->CR|= RNG_CR_RNGEN;
	
	if(RNG->SR & (RNG_SR_CECS|RNG_SR_SECS))
		return 1;
	else
		return 0;
}

uint32_t Random::GetInt(uint32_t max){
	uint32_t rnd=0;
	
	uosSynchronized(locker, {
		while(! ((RNG->SR & RNG_SR_DRDY) )){
			if(RNG->SR & RNG_SR_SECS)
				Init();
		}
		
		rnd=RNG->DR % max;
	});
	
	return rnd;
}

uint32_t Random::GetInt(){
	uint32_t rnd=0;
	
	uosSynchronized(locker, {
		while(! ((RNG->SR & RNG_SR_DRDY))){
			if(RNG->SR & RNG_SR_SECS)
				Init();
		}
		
		rnd=RNG->DR;
	});
	
	return rnd;
}

float Random::GetFloat(){
	float rnd=0;
	
	uosSynchronized(locker, {
		while(! ((RNG->SR & RNG_SR_DRDY))){
			if(RNG->SR & RNG_SR_SECS)
				ReInit();
		}
		
		rnd= ((float)(RNG->DR)) / ((float)0xFFFFFFFF);
	});
	
	return rnd;
}

void Random::ReInit(){
	RNG->SR= 0x00000000;
	RNG->CR&= ~RNG_CR_RNGEN;
	RNG->CR|= RNG_CR_RNGEN;
}