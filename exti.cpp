#include "exti.h"

void Exti::ConfigLineInterrupt(GPIO_TypeDef *seg, uint16_t line, uint8_t edge,  uint32_t preemptPriority, uint32_t subPriority){
	EXTI->IMR|= (uint32_t)0x01<<line;
	
	EXTI->FTSR|= (uint32_t)(edge&0x01)<<line;
	EXTI->RTSR|= (uint32_t)((edge&0x02)>>1)<<line;
	
	SetIRQSeg(TranslateGpioRegister(seg), line);
	
	if(line==EXTI_LINE_0)
		Nvic::EnableIRQ(EXTI0_IRQn, preemptPriority, subPriority);
	
	else if(line==EXTI_LINE_1)
		Nvic::EnableIRQ(EXTI1_IRQn, preemptPriority, subPriority);
	
	else if(line==EXTI_LINE_2)
		Nvic::EnableIRQ(EXTI2_IRQn, preemptPriority, subPriority);
	
	else if(line==EXTI_LINE_3)
		Nvic::EnableIRQ(EXTI3_IRQn, preemptPriority, subPriority);
	
	else if(line==EXTI_LINE_4)
		Nvic::EnableIRQ(EXTI4_IRQn, preemptPriority, subPriority);
	
	else if(line<=EXTI_LINE_9)
		Nvic::EnableIRQ(EXTI9_5_IRQn, preemptPriority, subPriority);
	
	else if(line<=EXTI_LINE_15)
		Nvic::EnableIRQ(EXTI15_10_IRQn, preemptPriority, subPriority);
}

uint32_t Exti::TranslateGpioRegister(GPIO_TypeDef *seg){
	if(seg==GPIOA)
		return EXTI_SEGMENT_A;
	else if(seg==GPIOB)
		return EXTI_SEGMENT_B;
	else if(seg==GPIOC)
		return EXTI_SEGMENT_C;
	else if(seg==GPIOD)
		return EXTI_SEGMENT_D;
	else if(seg==GPIOE)
		return EXTI_SEGMENT_E;
	else if(seg==GPIOF)
		return EXTI_SEGMENT_F;
	else if(seg==GPIOG)
		return EXTI_SEGMENT_G;
	else if(seg==GPIOH)
		return EXTI_SEGMENT_H;
	else if(seg==GPIOI)
		return EXTI_SEGMENT_I;
	else if(seg==GPIOJ)
		return EXTI_SEGMENT_J;
	else
		return EXTI_SEGMENT_K;
}

void Exti::SetIRQSeg(uint32_t seg, uint16_t line){
	if(line<EXTI_LINE_4)
		SYSCFG->EXTICR[0]|= seg<<(line*4);
	else if(line<EXTI_LINE_8)
		SYSCFG->EXTICR[1]|= seg<<((line-EXTI_LINE_4)*4);
	else if(line<EXTI_LINE_12)
		SYSCFG->EXTICR[2]|= seg<<((line-EXTI_LINE_8)*4);
	else
		SYSCFG->EXTICR[3]|= seg<<((line-EXTI_LINE_12)*4);
}