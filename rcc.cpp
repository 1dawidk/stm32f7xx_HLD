#include "rcc.h"

void Rcc::Deinit(){
}

uint8_t Rcc::SetPLL(uint32_t source, uint32_t pllm, uint32_t plln, uint32_t pllp, uint32_t pllq){
	uint32_t timeout;
	
	//Disable PLL
	RCC->CR&= ~RCC_CR_PLLON;
	
	//Wait till PLL is ready
	timeout= HLDKernel::GetTimeMs()+10;
	while(RCC->CIR & RCC_CIR_PLLRDYF){
		if(HLDKernel::GetTimeMs()>timeout) return RCC_TIMEOUT;
	}
	
	RCC->PLLCFGR= (0x20000000 | source | pllm | plln<<6 | pllp<<16 | pllq<<24);
	
	//Enable PLL
	RCC->CR|= RCC_CR_PLLON;
	
	//Wait till PLL is ready
	timeout= HLDKernel::GetTimeMs()+10;
	while(RCC->CIR & RCC_CIR_PLLRDYF){
		if(HLDKernel::GetTimeMs()>timeout) return RCC_TIMEOUT;
	}
	
	return RCC_OK;
}

uint8_t Rcc::TurnOnOsc(uint8_t osc){
	switch(osc){
		case RCC_OSCILLATOR_HSE:
			RCC->CR|= RCC_CR_HSEON;
			break;
		case RCC_OSCILLATOR_HSI:
			RCC->CR|= RCC_CR_HSION;
			break;
		case RCC_OSCILLATOR_LSE:
			RCC->BDCR|= RCC_BDCR_LSEON;
			break;
		case RCC_OSCILLATOR_LSI:
			RCC->CSR|=RCC_CSR_LSION;
			break;
	}
	
	return RCC_OK;
}

uint8_t Rcc::TurnOffOsc(uint8_t osc){
	switch(osc){
		case RCC_OSCILLATOR_HSE:
			RCC->CR&= ~RCC_CR_HSEON;
			break;
		case RCC_OSCILLATOR_HSI:
			RCC->CR&= ~RCC_CR_HSION;
			break;
		case RCC_OSCILLATOR_LSE:
			RCC->BDCR&= ~RCC_BDCR_LSEON;
			break;
		case RCC_OSCILLATOR_LSI:
			RCC->CSR&= ~RCC_CSR_LSION;
			break;
	}
	
	return RCC_OK;
}

uint8_t Rcc::SetAHBDivider(uint32_t divider){
	RCC->CFGR= (RCC->CFGR&0xFFFFFF0F) | ((uint32_t)divider<<4);
	
	return RCC_OK;
}

uint8_t Rcc::SetAPB1Divider(uint32_t divider){
	RCC->CFGR= (RCC->CFGR&0xFFFFE3FF) | ((uint32_t)divider<<10);
	
	return RCC_OK;
}

uint8_t Rcc::SetAPB2Divider(uint32_t divider){
	RCC->CFGR= (RCC->CFGR&0xFFFF1FFF) | ((uint32_t)divider<<13);
	
	return RCC_OK;
}

uint8_t Rcc::SetSystemClkSource(uint32_t source){
	uint32_t timeout= HLDKernel::GetTimeMs()+10;
	RCC->CFGR= (RCC->CFGR&0xFFFFFFFC) | (uint32_t)source;
	
	while( (RCC->CFGR&0x00000003) != (uint32_t)source )
		if(HLDKernel::GetTimeMs()>timeout) return RCC_TIMEOUT;
	
	return RCC_OK;
}

void Rcc::SetPeriphClkState(uint32_t periph, uint8_t state){
	switch(periph&RCC_BUS_TYPE_MASK){
		
		case RCC_BUS_AHB1:
			if(state)
				RCC->AHB1ENR|= 1<<(periph&RCC_PERIPHCLK_SHIFT_MASK);
			else
				RCC->AHB1ENR&= ~(1<<(periph&RCC_PERIPHCLK_SHIFT_MASK));
			break;
			
		case RCC_BUS_AHB2:
			if(state)
				RCC->AHB2ENR|= 1<<(periph&RCC_PERIPHCLK_SHIFT_MASK);
			else
				RCC->AHB2ENR&= ~(1<<(periph&RCC_PERIPHCLK_SHIFT_MASK));
			break;
		
		case RCC_BUS_AHB3:
			if(state)
				RCC->AHB3ENR|= 1<<(periph&RCC_PERIPHCLK_SHIFT_MASK);
			else
				RCC->AHB3ENR&= ~(1<<(periph&RCC_PERIPHCLK_SHIFT_MASK));
			break;
		
		case RCC_BUS_APB1:
			if(state)
				RCC->APB1ENR|= 1<<(periph&RCC_PERIPHCLK_SHIFT_MASK);
			else
				RCC->APB1ENR&= ~(1<<(periph&RCC_PERIPHCLK_SHIFT_MASK));
			break;
		
		case RCC_BUS_APB2:
			if(state)
				RCC->APB2ENR|= 1<<(periph&RCC_PERIPHCLK_SHIFT_MASK);
			else
				RCC->APB2ENR&= ~(1<<(periph&RCC_PERIPHCLK_SHIFT_MASK));
			break;
	}
}

void Rcc::SetDedicatedClkSrc(uint16_t periph, uint8_t source){
	if(periph&0x0100){
		RCC->DCKCFGR2|= (uint32_t)source << (periph&0x00FF);
	} else {
		RCC->DCKCFGR1|= (uint32_t)source << (periph&0x00FF);
	}
}
