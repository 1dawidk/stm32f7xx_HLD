#include "hldkernel.h"

uint32_t *HLDKernel::ms= new uint32_t(0);

void HLDKernel::delay_ms(uint32_t delay){
	uint32_t end=(*ms)+delay;
	
	while((*ms)<end);
}

uint32_t HLDKernel::GetTimeMs(){
	return (*ms);
}

void HLDKernel::IncTimeMs(){
	(*ms)++;
}